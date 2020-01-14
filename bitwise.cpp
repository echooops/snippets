#include <iostream>
#include <bitset>
using namespace std;

// 位运算的基本操作
int main(int argc, char *argv[])
{
    int x = 127;
    cout << "去掉最后一位: x >> 1" << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>(x >> 1) << "\n";

    cout << "在最后加一个 0: x << 1" << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>(x << 1) << "\n";

    cout << "在最后加一个 1: (x << 1) | 1" << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>((x << 1) | 1) << "\n";

    x = 128;
    cout << "把最后一位变成 1: x | 1" << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>(x | 1) << "\n";
    x = 127;
    cout << "把最后一位变成 0: (x | 1) - 1" << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>((x | 1) - 1) << "\n";

    cout << "最后一位取反: x ^ 1" << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>(x ^ 1) << "\n";

    x = 128;
    int k = 3;

    cout << "把右数第 K 位变成 1: x | (1 << (k - 1))" << " ----- k = " << k << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>(x | (1 << (k - 1))) << "\n";

    x = 127;
    cout << "把右数第 K 位变成 0: x & ~(1 << (k - 1))" << " ----- k = " << k << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>(x & ~(1 << (k - 1))) << "\n";

    cout << "右数第 K 位取反: x ^ (1 << (k - 1))" << " ----- k = " << k << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>(x ^ (1 << (k - 1))) << "\n";

    cout << "取末 K 位: x & ((1 << K) -1)" << " ----- k = " << k << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>(x ^ (1 << (k - 1))) << "\n";

    x = 4;
    cout << "右数第 K 位: x >> (k - 1) & 1" << " ----- k = " << k << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>(x >> (k - 1) & 1) << "\n";

    cout << "把末 K 变成 1: x ｜ ((1 << k) - 1)" << " ----- k = " << k << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>(x | ((1 << k) - 1)) << "\n";

    cout << "末 K 位取反: x ｜ ((1 << k) - 1)" << " ----- k = " << k << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>(x | ((1 << k) - 1)) << "\n";

    x = 79;
    cout << "把右边连续的 1 变成 0: x & (x + 1)" << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>(x & (x + 1)) << "\n";

    x = 80;
    cout << "把右边连续的 0 变成 1: x | (x - 1)" << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>(x | (x - 1)) << "\n";

    x = 79;
    cout << "把右起第一个 0 变成 1: x | (x + 1)" << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>(x | (x + 1)) << "\n";

    x = 80;
    cout << "把右起第一个 1 变成 0: x & (x - 1)" << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>(x & (x - 1)) << "\n";

    x = 76;
    cout << "取右边连续的 1: (x ^ (x + 1)) >> 1" << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>((x ^ (x + 1)) >> 1) << "\n";

    cout << "去掉起第一个 1 的左边: x & (x ^ (x - 1))" << "\n";
    cout << "树状数组中会用到..." << "\n";
    cout << bitset<8>(x) << "->" << bitset<8>(x & (x ^ (x - 1))) << "\n";

    return 0;
}
