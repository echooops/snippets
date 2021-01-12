#pragma once
#ifdef __cplusplus
extern "C"
{
#endif
    struct shmps_t {
        volatile size_t          size;              /* the size of the allocated buffer */
        volatile size_t          ext_size;
        volatile size_t          in;                /* data is added at offset (in % size) */
        volatile size_t          out;               /* data is extracted from off. (out % size) */
    };

    struct shmps_snapshot_t {
        size_t          size;         /* HEADER + DATA SIZE */
        unsigned char   data[0];
    };

    struct shmps_t *shm_init(const char *shm_name, size_t size, size_t ext_size);

    void shmps_free(struct shmps_t *shmps);

    /* 存取流式数据 */
    size_t shmps_put(struct shmps_t *shmps, const void *data, size_t size);
    size_t shmps_get(struct shmps_t *shmps, void *data, size_t size);

    /* 存取帧式数据 */
    size_t shmps_snapshot(struct shmps_t *shmps, void *data, size_t size);
    struct shmps_snapshot_t *shmps_snapshot_latest(struct shmps_t *shmps);

    size_t shmps_size(struct shmps_t *shmps);

    size_t shmps_space(struct shmps_t *shmps);

    void shmps_clear(struct shmps_t *shmps);

#ifdef __cplusplus
}
#endif
