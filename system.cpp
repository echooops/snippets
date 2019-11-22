#include <iostream>
#include <vector>

template<class... Args>
int system(Args&& ...args)
{
    std::vector<std::string> v = {args...};
    std::string command;
    for (auto &e : v) {
        command += e + " ";
    }
    return system(command.c_str());
}

int main(int argc, char *argv[])
{
    return system("ls", "-al");
}
