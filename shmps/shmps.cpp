#include <fcntl.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#if defined(__unix__) || defined(__linux__)
#include <sys/mman.h>
#include <unistd.h>
#else
#include <Windows.h>
#endif

#include "shmps.h"

#ifdef __cplusplus
extern "C"
{
#endif
#if defined(__unix__) || defined(__linux__)
    static size_t min(size_t lhs, size_t rhs)
    {
        return lhs < rhs ? lhs : rhs;
    }
#endif
    static int is_power_of_2(size_t num)
    {
        return !(num == 0) && !(num & (num - 1));
    }
    static size_t roundup_pow_of_two(size_t num)
    {
        int x = 0;
        while (num) { num >>= 1; x++; }
        return 1UL << x;
    }
    void *mkshm(const char *shm_name, size_t size)
    {
#if defined(__unix__) || defined(__linux__)
        void *buffer = NULL;
        int fd = open(shm_name, O_RDWR | O_CREAT, 0666);
        if (fd < 0) return NULL;
        do {
            if (ftruncate(fd, size) != 0) break;
            if (fsync(fd)) break;
            buffer = (struct mmps_t *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, fd, 0);
        }
        close(fd);
        return buffer;
#else
        HANDLE mapfile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, shm_name);
        if (mapfile == NULL) return NULL;
        LPVOID buffer = MapViewOfFile(mapfile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        CloseHandle(mapfile);
#endif
        return buffer;
    }
    struct shmps_t *shm_init(const char *shm_name, size_t size, size_t ext_size)
    {
        struct shmps_t *shmps = NULL;
        size = is_power_of_2(size) ? size : roundup_pow_of_two(size);
        size_t total_size = sizeof(struct shmps_t) + size + ext_size;
        if (shm_name == NULL) {
            shmps = (struct shmps_t *)malloc(total_size);
        } else {
            mkshm(shm_name, total_size);
        }
        if (shmps == NULL) return NULL;
        memset(shmps, 0, total_size);
        shmps->size = size;
        shmps->ext_size = ext_size;
        return shmps;
    }
    void shmps_free(struct shmps_t *shmps)
    {
        if (shmps != NULL) {
            free(shmps);
        }
    }
    /* 存取流式数据 */
    size_t shmps_put(struct shmps_t *shmps, const void *data, size_t size)
    {
        unsigned char *buffer = (unsigned char *)shmps + sizeof(struct shmps_t);
        size = min(size, shmps->size - shmps->in + shmps->out);
        size_t l = min(size, shmps->size - (shmps->in & (shmps->size - 1)));
        memcpy(buffer + (shmps->in & (shmps->size - 1)), data, l);
        memcpy(buffer, (unsigned char *)data + l, size - l);
        shmps->in += size;
        return size;
    }
    size_t shmps_get(struct shmps_t *shmps, void *data, size_t size)
    {
        unsigned char *buffer = (unsigned char *)shmps + sizeof(struct shmps_t);
        size = min(size, shmps->in - shmps->out);
        size_t l = min(size, shmps->size - (shmps->out & (shmps->size - 1)));
        memcpy(data, buffer + (shmps->out & (shmps->size - 1)), l);
        memcpy((unsigned char *)data + l, buffer, size - l);
        shmps->out += size;
        return size;
    }
    /* 存取帧式数据 */
    size_t shmps_snapshot(struct shmps_t *shmps, void *data, size_t size)
    {
        unsigned char *buffer = (unsigned char *)shmps + sizeof(struct shmps_t);
        size += sizeof(struct shmps_snapshot_t) + sizeof(size_t);
        if (shmps_space(shmps) < size) return NULL;
        struct shmps_snapshot_t *shmps_data = (struct shmps_snapshot_t *)(buffer + (shmps->in & (shmps->size - 1)));
        shmps_data->size = size;
        *(size_t *)((unsigned char *)shmps_data + size - sizeof(size_t)) = shmps->in;
        size_t l = min(shmps_data->size, shmps->size - (shmps->in & (shmps->size - 1)));
        memcpy(buffer, (const char *)shmps_data + l, shmps_data->size - l);
        shmps->in += shmps_data->size;
        return shmps_data->size;
    }
    struct shmps_snapshot_t *shmps_snapshot_latest(struct shmps_t *shmps)
    {
        unsigned char *buffer = (unsigned char *)shmps + sizeof(struct shmps_t);
        if (shmps_size(shmps) == 0) return NULL;
        shmps->out = *(size_t *)(buffer + ((shmps->in - sizeof(size_t)) & (shmps->size - 1)));
        return (struct shmps_snapshot_t *)(buffer + (shmps->out & (shmps->size - 1)));
    }
    size_t shmps_size(struct shmps_t *shmps)
    {
        return shmps->in - shmps->out;
    }
    size_t shmps_space(struct shmps_t *shmps)
    {
        return shmps->size - shmps_size(shmps);
    }
    void shmps_clear(struct shmps_t *shmps)
    {
        shmps->in = shmps->out = 0;
    }


#ifdef __cplusplus
}
#endif