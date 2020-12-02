
#include "mmps.h"
#include <fcntl.h>
#include <math.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>

static unsigned char * buffer;

static size_t min(size_t lhs, size_t rhs)
{
    return lhs < rhs ? lhs : rhs;
}

struct mmps_t *mmps_init(const char *filename, size_t size)
{
    struct mmps_t *p = NULL;
    /* ��֤ buffer ��С�� 2 ���� */
    size = is_power_of_2(size) ? size : roundup_pow_of_two(size);
    /* �� mmps �ṹ����ͷ */
    int fd = open(filename, O_RDWR | O_CREAT, 0666);
    if (fd < 0) return NULL;

    do {
        if (ftruncate(fd, size + sizeof(struct mmps_t)) != 0) break;
        if (fsync(fd)) break;
        p = (struct mmps_t *)mmap(NULL, size + sizeof(struct mmps_t), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, fd, 0);
        if (p == NULL) break;
        p->size = size;
        buffer = (unsigned char *)++p;
        // msync(p, sizeof(struct mmps_t), MS_SYNC);
    } while (0);
    close(fd);
    return p;
}

void mmps_free(struct mmps_t *mmps)
{
    munmap(mmps, mmps->size + sizeof(struct mmps_t));
}
size_t mmps_data_put(struct mmps_t *mmps, const struct mmps_data_t *mmps_data)
{
    if (mmps_space(mmps) < mmps_data->size) return 0;
    size_t len = mmps_data->size;
    /* buffer β ���� len �ĳ���ʱ��Ҫת�� buffer �� */
    size_t l = min(len, mmps->size - (mmps->in & (mmps->size - 1)));
    /* �������� */
    memcpy(buffer + (mmps->in & (mmps->size - 1)), mmps_data, l);
    memcpy(buffer, (const char *)mmps_data + l, len - l);
    /* ÿ���ۼ�д�������������ֵ�����*/
    mmps->in += len;
    return len;
}

size_t mmps_data_get(struct mmps_t *mmps, struct mmps_data_t **mmps_data)
{
    /* �Ȼ�ȡ����ͷ */
    if (mmps_size(mmps) == 0) {
        *mmps_data = NULL;
        return 0;
    }
    size_t mmps_data_size = 0;
    size_t hl = min(sizeof(size_t), mmps->size - (mmps->out & (mmps->size - 1)));
    memcpy(&mmps_data_size, buffer + (mmps->out & (mmps->size - 1)), hl);
    memcpy((char *)&mmps_data_size + hl, buffer, sizeof(size_t) - hl);
    /* ��ȡ���� */
    *mmps_data = (struct mmps_data_t *)malloc(mmps_data_size);
    size_t len = mmps_data_size;
    size_t l = min(len, mmps->size - (mmps->out & (mmps->size - 1)));
    /* ��ȡ buffer β�ͣ��� buffer ͷ������ */
    memcpy(*mmps_data, buffer + (mmps->out & (mmps->size - 1)), l);
    memcpy((char *)*mmps_data + l, buffer, len - l);
    /* �������ݻ���Ϣ */
    mmps->out += len;
    return len;
}

struct mmps_data_t *mmps_data_alloc(size_t data_size)
{
    struct mmps_data_t *p = malloc(data_size + sizeof(size_t));
    if (p != NULL) p->size = data_size + sizeof(size_t);
    return p;
}

void mmps_data_free(const struct mmps_data_t *mmps_data)
{
    if (mmps_data != NULL) free((void *)mmps_data);
}

size_t mmps_size(struct mmps_t *mmps)
{
    /* ������ uint �����ǻ��ε� */
    return mmps->in - mmps->out;
}
size_t mmps_space(struct mmps_t *mmps)
{
    /* ������ uint �����ǻ��ε� */
    return mmps->size - mmps_size(mmps);
}
void mmps_clear(struct mmps_t *mmps)
{
    mmps->in = mmps->out = 0;
    // msync(mmps, sizeof(struct mmps_t), MS_SYNC);
}

int is_power_of_2(size_t num)
{
    return !(num == 0) && !(num & (num - 1));
}
size_t roundup_pow_of_two(size_t num)
{
    int x = 0;
    while (num) { num >>= 1; x++; }
    return 1UL << x;
}
