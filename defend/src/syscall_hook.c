#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "syscall_hook.h"

int defend_assert(const char *path)
{
    if (strncmp(path, "/opt/COMMON/VRV/DYKL", strlen("/opt/COMMON/VRV/DYKL")) == 0) {
        return 0;
    }
    if (strncmp(path, "/opt/DYSJ/VRV", strlen("/opt/DYSJ/VRV")) == 0) {
        return 0;
    }
    if (strncmp(path, "/opt/KLSJ/VRV", strlen("/opt/KLSJ/VRV")) == 0) {
        return 0;
    }

    return -1;
}

int rename_hook(const char *oldpath, const char *newpath)
{
    char realoldpath[BUFSIZ] = {0};
    if (realpath(oldpath, realoldpath) == NULL)
        return 0;
    if (defend_assert(realoldpath) == 0)
        return -1;
    if (newpath == NULL)
        return 0;

    char path[BUFSIZ] = {0};
    if (getcwd(path, BUFSIZ) == NULL)
        return 0;

    char realnewpath[BUFSIZ] = {0};
    char *temp = strrchr(newpath, '/');
    if (temp == NULL) {
        snprintf(realnewpath, BUFSIZ - 1, "%s/%s", path, newpath);
    } else {
        if (newpath[0] == '/') {
            strcpy(realnewpath, newpath);
        } else {
            strcat(path, "/");
            strcat(path, newpath);
            realpath(newpath, realnewpath);
        }
    }
    if (defend_assert(realnewpath) == 0)
        return -1;

    return 0;
}

int renameat_hook(int olddirfd, const char *oldpath, int newdirfd, const char *newpath)
{
    printf("renameat");
    return rename_hook(oldpath, newpath);
}

int renameat2_hook(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, unsigned int flags)
{
    return renameat_hook(olddirfd, oldpath, newdirfd, newpath);
}

int unlink_hook(const char *pathname)
{
    char realpathname[BUFSIZ] = {0};
    if (realpath(pathname, realpathname) == NULL)
        return 0;
    if (defend_assert(realpathname) == 0)
        return -1;
    return 0;
}

int unlinkat_hook(int dirfd, const char *pathname, int flags)
{
    return unlink_hook(pathname);
}

int kill_hook(pid_t pid, int sig)
{
    if (getpid() == 1) return 0;
    char temp[BUFSIZ] = {0};
    char path[BUFSIZ] = {0};
    // atoi(argv[1]) 换成pid
    snprintf(temp, BUFSIZ - 1, "/proc/%d/exe", pid);
    ssize_t size = 0;
    if ((size = readlink(temp, path, BUFSIZ)) < 0) {
        return -1;
    }
    path[size] = '\0';
    if (defend_assert(path) == 0)
        return -1;
    return 0;
}
