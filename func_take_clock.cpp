#include <iostream>
#include <iterator>
#include <functional>
#include <numeric>              // 数值算法
#include <algorithm>            // 基本算法

#include <ctime>
#include <cstring>

using namespace std;


#if defined (__i386__)
static __inline__ unsigned long long
rdtsc(void)
{
    unsigned long long x;
    __asm__ volatile("rdtsc":"=A"(x));
    return x;
}

#elif defined (__x86_64__)
static __inline__ unsigned long long
rdtsc(void)
{
    unsigned hi,lo;
    __asm__ volatile("rdtsc":"=a"(lo),"=d"(hi));
    return ((unsigned long long)lo) | (((unsigned long long)hi)<<32);
}
#endif

void psort(int *first, int *last)
{
    int max , min;
    min = max = *first;
    int * temp = first;
    while(first != last) {
        if(min > *first) min = *first;
        if(max < *first) max = *first;
        first++;
    }
    int **point = (int **)malloc(sizeof(void*) * (max - min + 1));
    memset(point, 0, sizeof(void*) * (max - min + 1));
    first = temp;
    while(first != last) {
        point[*first - min] = first;
        first ++;
    }
    first = temp;
    int ** p_temp = point;
    while(first != last) {
        if(*point != NULL) {
            *first = **point;
            first++;
        }
        point++;
    }
    free(p_temp);
}

template <class F, class... Args>
double func_take_clock(F&& f, Args&&... args)
{
    clock_t start = clock();

    auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    func();
    clock_t end = clock();
    return double(end - start) / CLOCKS_PER_SEC ;
}


template <class RandowAccessIterator, class Function>
double sort_take_clock(RandowAccessIterator first, RandowAccessIterator last,
                       Function f)
{
    clock_t start = clock();

    f(first, last);

    clock_t end = clock();
    return double(end - start) / CLOCKS_PER_SEC ;
}

#define TEST_NUM 100


int main()
{
    int* a = (int *)malloc(sizeof(int) * TEST_NUM);
    for(int i = 0; i < TEST_NUM; i++) {
        a[i] = i;
    }
    srand(unsigned(time(NULL)));

    random_shuffle(a, a + TEST_NUM);

    auto fun = [](int val) {cout << val << " "; }; // lambda 函数

    for_each(a, a + 9, fun);
    copy(a + 9, a + 19, ostream_iterator<int>(cout, " "));
    cout << endl;

    cout << "stl sort = " << func_take_clock(sort<int *>, a, a + TEST_NUM) << endl;
    cout << "stl sort = " << sort_take_clock(a, a + TEST_NUM, sort<int *>) << endl;
    copy(a, a + 9, ostream_iterator<int>(cout, " "));
    cout << endl;
    random_shuffle(a, a + TEST_NUM);

    cout << "point sort = " << func_take_clock(psort, a, a + TEST_NUM) << endl;

    copy(a, a + 9, ostream_iterator<int>(cout, " "));
    cout << endl;

    return 0;
}
