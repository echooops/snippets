#pragma once
#include <atomic>
#include <mutex>

class QBuffer
{
public:
    QBuffer(size_t size, size_t tails) : m_size(size), m_tails(tails)
    {
        /* 保证 buffer 大小是 2 的幂 */
        m_size = IsPowerOfTwo(size) ? size : RoundupPowOfTwo(size);
        /* 申请环形队列大小 */
        m_buffer = calloc(m_size + m_tails, sizeof(char));

        m_space = 12;                                       /* 安全距离 */
    }
    virtual ~QBuffer()
    {
        if (m_buffer != nullptr) {
            free(m_buffer);
        }
    }
    /* 存数据 */
    void *GetPushBuffer(size_t size)
    {
        while (RemainingSize() < size) {                    /* 扔数据 */
            DropOne();
        }
        size_t *pos = (size_t *)BufferIn();                 /* 取地址 */
        *pos = size + sizeof(size_t) + m_space;
        return (char *)(++pos) + m_space;                   /* 返回数据部 */
    }
    void PushFinish(size_t size)
    {
        size_t *pos = (size_t *)BufferIn();
        if (*pos != size + sizeof(size_t) + m_space) {      /* 校验 */
            *pos = size + sizeof(size_t) + m_space;
        }
        m_in += *pos;
        m_readCv.notify_all();
    }
    /* 取数据 */
    void *GetPullBuffer(size_t &size, int timeout = 500)
    {
        if (UsedSize() <= 0) {
            std::unique_lock<std::mutex> lock(m_dropMutex);
            if (m_readCv.wait_for(lock, std::chrono::milliseconds(timeout)) == std::cv_status::timeout) {
                size = 0;
                return nullptr;
            }
        }
        m_dropMutex.lock();
        size_t *pos = (size_t *)BufferOut();                /* 取地址 */
        size = *pos - sizeof(size_t) - m_space;             /* 取大小 */
        return (char *)(++pos) + m_space;                   /* 返回数据部 */
    }
    void PullFinish(size_t size)
    {
        size_t *pos = (size_t *)BufferOut();                /* 取地址 */
        m_out += *pos;
        m_dropMutex.try_lock();
        m_dropMutex.unlock();
    }
    void Clear()
    {
        memset(m_buffer, 0, m_size);
        m_in = m_out = 0;
    }
    size_t RemainingSize()
    {
        return m_size - UsedSize();
    }
    size_t UsedSize()
    {
        return m_in - m_out;
    }
private:
    void DropOne()
    {
        if (m_dropMutex.try_lock()) {
            size_t *pos = (size_t *)BufferOut();
            LOG_ERROR("drop buffer size : {}.", *pos);
            m_out += *pos;
            m_dropMutex.unlock();
        }
    }
    void *BufferIn()
    {
        return (void *)((char *)m_buffer + (m_in & (m_size - 1)));
    }
    void *BufferOut()
    {
        return (void *)((char *)m_buffer + (m_out & (m_size - 1)));
    }
    bool IsPowerOfTwo(size_t size)
    {
        return !(size == 0) && !(size & (size - 1));
    }
    size_t RoundupPowOfTwo(size_t size)
    {
        size_t x = 0;
        while (size) {
            size >>= 1;
            x++;
        }
        return 1UL << x;
    }
private:
    std::atomic<size_t>         m_in{0};
    std::atomic<size_t>         m_out{0};
    size_t                      m_size{0};
    size_t                      m_tails{0};                 /* Q 性队列的魔性尾巴 */
    size_t                      m_space{0};                 /* 安全空间 */
    void                        *m_buffer{nullptr};
    std::mutex                  m_dropMutex;
    std::condition_variable     m_readCv;
};
