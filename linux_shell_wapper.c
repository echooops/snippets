#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
/* 清空目录
 * recursive: 0 清空当前目录文件, 1 递归清空子目录
 */
int RemoveDir(const char *pathname, int recursive)
{
    char path[PATH_MAX] = {0};
    if (rmdir(pathname) == 0) {
        return 0;
    }
    switch (errno) {
        case ENOTDIR: {
            return unlink(pathname);
        }
        case ENOTEMPTY: {
            DIR *d = opendir(pathname);
            if (d == NULL) {
                return -1;
            }
            struct dirent *dir;
            while (dir = readdir(d), dir) {
                if (dir->d_name[0] == '.' &&
                    (dir->d_name[1] == '\0' || (dir->d_name[1] == '.' && dir->d_name[2] == '\0'))) {    // 偏移 2
                    continue;  // 跳过 . and ..
                }
                if (snprintf_s(path, PATH_MAX, PATH_MAX - 1, "%s/%s", pathname, dir->d_name) < 0) {
                    closedir(d);
                    return -1;
                }
                // 递归删除子目录
                if (recursive && RemoveDir(path, recursive) < 0) {
                    closedir(d);
                    return -1;
                }
            }
            closedir(d);
            if (rmdir(pathname) < 0) {
                return -1;
            }
            break;
        }
        default:
            return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    return RemoveDir(argv[1], 1);
}
