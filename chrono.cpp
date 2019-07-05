#include <chrono>
#include <iostream>
#include <functional>
#include <future>

// 计算函数耗时
template <class F, class... Args>
auto func_takes(F&& func, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{

    using RetType = typename std::result_of<F(Args...)>::type;
    //using RetType = decltype(func(args...));

    auto task = std::make_shared<std::packaged_task<RetType()>>(
        std::bind(std::forward<F>(func), std::forward<Args>(args)...)
        );

    std::future<RetType> future = task->get_future();

    // 执行函数
    std::thread([task](){
                    std::chrono::high_resolution_clock::time_point p0 = std::chrono::high_resolution_clock::now();
                    // std::chrono::time_point<std::chrono::high_resolution_clock> p0 = std::chrono::high_resolution_clock::now();
                    (*task)();
                    std::chrono::high_resolution_clock::time_point p1 = std::chrono::high_resolution_clock::now();
                    // std::chrono::time_point<std::chrono::high_resolution_clock> p1 = std::chrono::high_resolution_clock::now();

                    //计算及打印耗时，用法不太标准,文中的1000 是换算到毫秒的意思
                    uint64_t stitch = std::chrono::duration_cast<std::chrono::microseconds>(p1 - p0).count();
                    std::cout << "stitch high_resolution_clock time:" << stitch / 1000.0 << " ms" << std::endl;;
                }).detach();
    return future;
}

int main(int argc, char *argv[])
{
    auto func = [](const char *s) {
                    std::cout << s << "\n";
                    return 0;
                };
    auto ret = func_takes(func, "Hello World");
    std::cout << "wait ......" << "\n";
    auto result = ret.get();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return 0;
}
