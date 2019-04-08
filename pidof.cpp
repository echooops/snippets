#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <set>
// 不需要root权限
// 获取单个进程ID
pid_t pidof(const char *pname)
{
    char cmd[BUFSIZ] = {0};
    char line[BUFSIZ] = {0};
    snprintf(cmd, BUFSIZ - 1, "pgrep %s", pname);
    FILE* fp = popen(cmd, "r");
    if (fp == NULL) return -1;
    char *spid = fgets(line, BUFSIZ, fp);
    pclose(fp);
    if (spid == NULL) return -1;
    return atoi(spid);
}

typedef std::set<pid_t> pids_t;
// 获取一组进程ID
pids_t pgrep(const char *pname)
{
    char cmd[BUFSIZ] = {0};
    char line[BUFSIZ] = {0};
    pids_t pids;
    snprintf(cmd, BUFSIZ - 1, "pgrep %s", pname);
    FILE* fp = popen(cmd, "r");
    if (fp == NULL) return pids;
    char *spid = nullptr;
    while((spid = fgets(line, BUFSIZ, fp)))
        pids.insert(atoi(spid));
    pclose(fp);
    return pids;
}

int main(int argc, char *argv[])
{
    pid_t pid = pidof(argv[1]);
    std::cout << pid << "\n";

    std::cout << "-----------------" << "\n";
    pids_t pids = pgrep(argv[1]);
    
    for (auto &e : pids)
        std::cout << e << "\n";
    
    return 0;
}
