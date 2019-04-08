#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

// 需要root权限支持
std::string pnameof(pid_t pid)
{
    char temp[BUFSIZ] = {0};
    char path[BUFSIZ] = {0};
    // atoi(argv[1]) 换成pid
    snprintf(temp, BUFSIZ - 1, "/proc/%d/exe", pid);
    ssize_t size = 0;
    if ((size = readlink(temp, path, BUFSIZ)) < 0) {
        // 没读到
        return "";
    }
    path[size] = '\0';
    char * pos = strrchr(path, '/');
    if (pos == NULL) {
        // 读到的内容有问题
        return "";
    }
    // path 指向全路径， pos指向进程名
    pos++;
    return pos;
}


int main(int argc, char *argv[])
{
    std::cout << pnameof(atoi(argv[1])) << "\n";
    return 0;
}
