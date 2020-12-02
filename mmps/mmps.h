#ifndef MMPS_H
#define MMPS_H
#include "string.h"

struct mmps_t {
    size_t          size;         /* the size of the allocated buffer */
    size_t          in;           /* data is added at offset (in % size) */
    size_t          out;          /* data is extracted from off. (out % size) */
};

struct mmps_data_t {
    size_t          size;         /* HEADER + DATA SIZE */
    unsigned char   data[0];
};

struct mmps_t *mmps_init(const char *filename, size_t size);

void mmps_free(struct mmps_t *mmps);

size_t mmps_data_put(struct mmps_t *mmps, const struct mmps_data_t *mmps_data);

size_t mmps_data_get(struct mmps_t *mmps, struct mmps_data_t **mmps_data);

struct mmps_data_t *mmps_data_alloc(size_t data_size);

void mmps_data_free(const struct mmps_data_t *mmps_data);

/* --------------- */
size_t mmps_size(struct mmps_t *mmps);

size_t mmps_space(struct mmps_t *mmps);

void mmps_clear(struct mmps_t *mmps);

int is_power_of_2(size_t num);

size_t roundup_pow_of_two(size_t num);


#endif // !MMPS_H
