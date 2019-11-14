#ifndef _SYSCALL_HOOK_H_
#define _SYSCALL_HOOK_H_


int defend_assert(const char *path);

int rename_hook(const char *oldpath, const char *newpath);
int renameat_hook(int olddirfd, const char *oldpath, int newdirfd, const char *newpath);
int renameat2_hook(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, unsigned int flags);

int unlink_hook(const char *pathname);
int unlinkat_hook(int dirfd, const char *pathname, int flags);

int kill_hook(pid_t pid, int sig);

#endif  /* _SYSCALL_HOOK_H_ */
