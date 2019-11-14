#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <errno.h>

#include <stdio.h>

#include "syscall_hook.h"

static const char g_evil_env[] = "VRV_DYKL_DEFEND_ENVAR";

static int g_hook_flag = 1;

#define DLSYM_FUNC(rt, symbol, ...)                                     \
    extern int symbol##_hook(__VA_ARGS__);                              \
    rt symbol(__VA_ARGS__)                                              \
    {                                                                   \
        int temp_errno = errno;                                         \
        static typeof(&(symbol)) func_##symbol = NULL;                  \
        if (!func_##symbol) {                                           \
            func_##symbol = (typeof(&(symbol))) dlsym(RTLD_NEXT, #symbol); \
        }                                                               \
        int ret = 0;                                                    \
        if (g_hook_flag) {                                              \
            ret = symbol##_hook(symbol##_args);                         \
        }                                                               \
        if (ret) {                                                      \
            errno = EPERM;                                              \
            return -errno;                                              \
        }                                                               \
        errno = temp_errno;                                             \
        if (!func_##symbol) return -1;                                  \
        return func_##symbol(symbol##_args);                            \
    }

/* 打开 hook */
void open_syscall_hook(void)
{
    g_hook_flag = 1;
}
/* 关闭 hook */
void close_syscall_hook(void)
{
    g_hook_flag = 0;
}

static void __attach(void) __attribute__ ((constructor));
static void __detach(void) __attribute__ ((destructor));

static void __attach(void)
{
    char path[BUFSIZ] = {0};
    ssize_t size = 0;
    /* 放行匹配到的自己 */
    if ((size = readlink("/proc/self/exe", path, BUFSIZ)) < 0)
        return;
    if (defend_assert(path) == 0)
        close_syscall_hook();

    char temp[BUFSIZ] = {0};
    memset(path, 0, BUFSIZ);
    snprintf(temp, BUFSIZ, "/proc/%d/exe", getppid());
    /* 放行匹配父进程 */
    if ((size = readlink(temp, path, BUFSIZ)) < 0)
        return;
    if (defend_assert(path) == 0)
        close_syscall_hook();

    if (strstr(path, "sshd"))   /* 关闭对 sshd 的控制 */
        close_syscall_hook();

    if (getenv(g_evil_env) != NULL)
        close_syscall_hook();

    uid_t uid = getuid();
    if(0 < uid && uid < 1000 )
        close_syscall_hook();
}

static void __detach(void)
{
    close_syscall_hook();
}

/* 防移动 */
/* rename 1.0 change the name or location of a file */
#define rename_args oldpath, newpath
DLSYM_FUNC(int, rename, const char *oldpath, const char *newpath)
/* renameat 2.6.16 */
#define renameat_args olddirfd, oldpath, newdirfd, newpath
DLSYM_FUNC(int, renameat, int olddirfd, const char *oldpath, int newdirfd, const char *newpath)
/* renameat2 3.15 */
#define renameat2_args olddirfd, oldpath, newdirfd,  newpath, flags
DLSYM_FUNC(int, renameat2, int olddirfd, const char *oldpath, int newdirfd, const char *newpath, unsigned int flags)

/* 防删 */
/* unlink 1.0 删除一个名称，可能是它引用的文件 */
#define unlink_args pathname
DLSYM_FUNC(int, unlink, const char *pathname)
/* unlinkat 2.6.16 delete a name and possibly the file it refers to */
#define unlinkat_args dirfd, pathname, flags
DLSYM_FUNC(int, unlinkat, int dirfd, const char *pathname, int flags)

/* 防杀 */
#define kill_args pid, sig
DLSYM_FUNC(int, kill, pid_t pid, int sig)
