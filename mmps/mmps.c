
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
    /* 保证 buffer 大小是 2 的幂 */
    size = is_power_of_2(size) ? size : roundup_pow_of_two(size);
    /* 含 mmps 结构描述头 */
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
    /* buffer 尾 不够 len 的长度时需要转到 buffer 首 */
    size_t l = min(len, mmps->size - (mmps->in & (mmps->size - 1)));
    /* 拷贝数据 */
    memcpy(buffer + (mmps->in & (mmps->size - 1)), mmps_data, l);
    memcpy(buffer, (const char *)mmps_data + l, len - l);
    /* 每次累加写入量，到达最大值后溢出*/
    mmps->in += len;
    return len;
}

size_t mmps_data_get(struct mmps_t *mmps, struct mmps_data_t **mmps_data)
{
    /* 先获取数据头 */
    if (mmps_size(mmps) == 0) {
        *mmps_data = NULL;
        return 0;
    }
    size_t mmps_data_size = 0;
    size_t hl = min(sizeof(size_t), mmps->size - (mmps->out & (mmps->size - 1)));
    memcpy(&mmps_data_size, buffer + (mmps->out & (mmps->size - 1)), hl);
    memcpy((char *)&mmps_data_size + hl, buffer, sizeof(size_t) - hl);
    /* 获取数据 */
    *mmps_data = (struct mmps_data_t *)malloc(mmps_data_size);
    size_t len = mmps_data_size;
    size_t l = min(len, mmps->size - (mmps->out & (mmps->size - 1)));
    /* 读取 buffer 尾巴，和 buffer 头的数据 */
    memcpy(*mmps_data, buffer + (mmps->out & (mmps->size - 1)), l);
    memcpy((char *)*mmps_data + l, buffer, len - l);
    /* 更新数据环信息 */
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
    /* 利用了 uint 递增是环形的 */
    return mmps->in - mmps->out;
}
size_t mmps_space(struct mmps_t *mmps)
{
    /* 利用了 uint 递增是环形的 */
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
