#include <iostream>
#include <cstdlib>
#include <unistd.h>

// ------ 检查并写入 ld_preload 保护库 ------- //
static char libdefend_path[] = "/usr/lib/libdefend.so";
static char ldpreload_path[] = "/etc/ld.so.preload";
static int ldpreload_defend_init(void) __attribute__ ((constructor));
static int ldpreload_defend_uninit(void) __attribute__ ((destructor));

static int ldpreload_defend_uninit(void)
{
    if (access(ldpreload_path, F_OK) == 0) {
        char cmd[BUFSIZ] = {0};
        snprintf(cmd, BUFSIZ - 1,"sed -i \"/\\/usr\\/lib/d\" %s", ldpreload_path);
        return system(cmd);
    }
    return 0;
}

static int ldpreload_defend_init(void)
{
    if (access(ldpreload_path, F_OK) == 0) {
        ldpreload_defend_uninit();
    }
    char cmd[BUFSIZ] = {0};
    snprintf (cmd, BUFSIZ - 1, "echo \"%s\" >> %s", libdefend_path, ldpreload_path);
    return system(cmd);
}

int main(int argc, char *argv[])
{
    sleep(1000);
    return 0;
}
