#include <mutex>
#include <iostream>

#define SINGLETON_CTOR(x) \
    private: \
        x() = default; \
        x(const x&) = delete; \
        x& operator=(const x&) = delete; \
        ~x() = default;

/* CRTP 递归模板模式
 * 1、继承自模板类
 * 2、派生类将自身作为参数传给模板类
 */
template <typename T>
class SingletonBase
{
    SINGLETON_CTOR(SingletonBase)
public:
    static T& Instance()
    {
        static std::once_flag of;
        std::call_once(of, [&]() { m_ptr.reset(static_cast<T*>(new SingletonBase)); });
        return *m_ptr;
    }
private:
    static std::unique_ptr<T> m_ptr;
};
template <typename T>
std::unique_ptr<T> SingletonBase<T>::m_ptr;

class Singleton : public SingletonBase<Singleton>
{
public:
    void test() { std::cout << "test\n"; };
};
