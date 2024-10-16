#pragma once
#include <vector>
#include <atomic>
/* 只有一个生产者和一个消费者则是线程安全的 */
template <typename T>
class RingFIFO {
public:
    std::vector<T> ring;
    size_t size;
    std::atomic<size_t> in;
    std::atomic<size_t> out;

public:
    RingFIFO(size_t num) : size(num), in(0), out(0)
    {
        /* 保证 buffer 大小是 2 的幂 */
        size = is_power_of_2(num) ? size : roundup_pow_of_two(num);
        /* 申请环形队列大小 */
        ring.resize(size);
    }
    ~RingFIFO()
    {
    }
    [[nodiscard]] bool Push(T &data) noexcept
    {
        if (full())
            return false;
        size_t pos = in & (size - 1);
        ring[pos].swap(data);
        in++;
        return true;
    }
    [[nodiscard]] bool Pop(T &data) noexcept
    {
        if (empty())
            return false;
        size_t pos = out & (size - 1);
        data.swap(ring[pos]);
        out++;
        return true;
    }
    [[nodiscard]] size_t Size() const noexcept
    {
        /* 利用了 uint 递增是环形的 */
        return in - out;
    }
    void Clear() noexcept
    {
        in = out = 0;
    }

private:
    bool full() const noexcept
    {
        return (in - out) == size;
    }
    bool empty() const noexcept
    {
        return in == out;
    }
    bool is_power_of_2(size_t num) const noexcept
    {
        return !(num == 0) && !(num & (num - 1));
    }
    size_t roundup_pow_of_two(size_t num) const noexcept
    {
        int x = 0;
        while (num) {
            num >>= 1;
            x++;
        }
        return 1UL << x;
    }
};
