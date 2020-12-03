#include <stdio.h>
#include <unistd.h>

#include "mmps.h"

int main(int argc, char *argv[])
{

    struct mmps_t *p = mmps_init("/tmp/tmp.txt", 1024 * 1024 * 50);

    mmps_clear(p);

    size_t size = 0;
    while (1) {
        usleep(10000);
        size = rand() % 1024 * 1024 * 10;
        struct mmps_data_t *pp = mmps_data_alloc(size);
        if (mmps_data_put(p, pp) == 0) {
            printf("put error, no space .. [mmps size : %zu, in : %zu, out : %zu]\n", p->size, p->in, p->out);
        } else {
            printf("put data size : %zu ...[mmps total : %zu, size : %zu, space : %zu]\n",
                   pp->size, p->size, mmps_size(p), mmps_space(p));
        }
        mmps_data_free(pp);
    }
    return 0;
}
