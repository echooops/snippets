#include "mmps.h"
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    struct mmps_t *p = mmps_init("/tmp/tmp.txt", 1024 * 1024 * 10);
    printf("hello\n");
    mmps_clear(p);
    struct mmps_data_t *pp;
    while (1) {
        usleep(10000);
        if (mmps_data_get(p, &pp) == 0) {
            printf("get error, empty mmps .[mmps size : %zu, in : %zu, out : %zu]\n", p->size, p->in, p->out);
        } else {
            printf("get data size : %zu ...[mmps size : %zu, in : %zu, out : %zu]\n", pp->size, p->size, p->in, p->out);
            mmps_data_free(pp);
        }
    }
    return 0;
}
