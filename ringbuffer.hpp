#pragma once
#include <cstdint>
#include <algorithm>

/* 只有一个生产者和一个消费者则是线程安全的 */
template<typename T>
class ringbuffer {

    T       *buffer;
    size_t  size;
    size_t  in;
    size_t  out;

public:
    ringbuffer(size_t num) : buffer(nullptr), size(num), in(0), out(0)
    {
        /* 保证 buffer 大小是 2 的幂 */
        size = is_power_of_2(num) ? size : roundup_pow_of_two(num);
        /* 申请环形队列大小 */
        buffer = (T *)malloc(size * sizeof(T));
    }
    ~ringbuffer()
    {
        if (buffer != nullptr) {
            free(buffer);
            buffer = nullptr;
        }
    }
    size_t put(const T *data, size_t len)
    {
        len = std::min(len, size - in + out);
        /* buffer 尾 不够 len 的长度时需要转到 buffer 首 */
        size_t l = std::min(len, size - (in & (size - 1)));
        /* 拷贝数据 */
        memcpy(buffer + (in & (size - 1)), data, l * sizeof(T) / sizeof(char));
        memcpy(buffer, data + l, (len - l) * sizeof(T) / sizeof(char));
        /* 每次累加写入量，到达最大值后溢出*/
        in += len;
        return len;
    }
    size_t get(T *data, size_t len)
    {
        len = std::min(len, in - out);
        size_t l = std::min(len, size - (out & (size - 1)));

        /* 读取 buffer 尾巴，和 buffer 头的数据 */
        memcpy(data, buffer + (out & (size - 1)), l * sizeof(T) / sizeof(char));
        memcpy(data + l, buffer, (len - l) * sizeof(T) / sizeof(char));

        /* 每次累加，到达最大值后溢出，自动转为 0 */
        out += len;
        return len;
    }
    size_t len()
    {
        /* 利用了 uint 递增是环形的 */
        return in - out;
    }
    void clear()
    {
        in = out = 0;
    }
private:
    bool is_power_of_2(size_t num)
    {
        return !(num == 0) && !(num & (num - 1));
    }
    size_t roundup_pow_of_two(size_t num)
    {
        int x = 0;
        while (num) { num >>= 1; x++; }
        return 1UL << x;
    }
};
