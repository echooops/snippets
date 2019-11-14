#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <unistd.h>

// 设置工作路径为进程所在路径
int chpwkdir(void)
{
    char path[BUFSIZ] = {0};
    ssize_t size = 0;
    if ((size = readlink("/proc/self/exe", path, BUFSIZ)) < 0) {
        std::cerr << "[ERROR] readlink : " << strerror(errno) << "\n";
        return -1;
    }
    while(path[size] != '/') {
        path[size--] = '\0';
    }
    std::cout << "进程所在目录 : " << path << "\n";
    if (0 != chdir(path)) {
        std::cerr << "[ERROR] chdir : " << strerror(errno) << "\n";
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (0 == chpwkdir())
        std::cout << "修改工作目录为进程所在目录..." << "\n";
    char path[BUFSIZ] = {0};
    std::cout << getcwd(path, BUFSIZ) << "\n";
    return 0;
}
