#ifndef WRAPPER_SPDLOG_WELOG_H
#define WRAPPER_SPDLOG_WELOG_H

#include <filesystem>
#include <sstream>
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/logger.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/bundled/printf.h>

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace welog {

namespace fs = std::filesystem;

class logger final {
public:
    // 流式输出
    struct log_stream : public std::ostringstream {
        log_stream(const spdlog::source_loc &loc, spdlog::level::level_enum lvl, std::string_view prefix)
            : loc_(loc), lvl_(lvl), prefix_(prefix)
        {}
        ~log_stream()
        {
            flush();
        }
        void flush()
        {
            logger::get().log(loc_, lvl_, (prefix_ + str()).c_str());
        }
    private:
        spdlog::source_loc loc_;
        spdlog::level::level_enum lvl_;
        std::string prefix_;
    };
public:
    static logger &get()
    {
        static logger logger;
        return logger;
    }
    bool init(std::string_view filename)
    {
        if (is_inited_) return true;
        try {
            // 检查目录是否存在
            fs::path log_path(filename);
            fs::path log_dir = log_path.parent_path();
            if (!fs::exists(log_path)) {
                fs::create_directories(log_dir);
            }
            // 初始化日志缓存
            constexpr std::size_t log_buffer_size = 8 * 1024; // 8 kb

            spdlog::init_thread_pool(log_buffer_size, std::thread::hardware_concurrency());
            std::vector<spdlog::sink_ptr> sinks;
#if 1
            // 每天一个日志，3 天便利店
            auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(log_path.string(), 0, 2, false, 3);
            sinks.push_back(daily_sink);
#else
            auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_path.string(), log_buffer_size, 3);
            sinks.push_back(rotating_sink);
#endif

#if defined(_DEBUG) && defined(WIN32)
            auto ms_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
            sinks.push_back(ms_sink);
#endif //  _DEBUG

#ifdef _LOG_CONSOLE
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            sinks.push_back(console_sink);
#endif
            spdlog::set_default_logger(std::make_shared<spdlog::logger>("", sinks.begin(), sinks.end()));
            spdlog::set_pattern("[%L %D %T.%e %P %t %!] %s(%#): %v");
            spdlog::flush_on(spdlog::level::warn);
            spdlog::set_level(log_level_);
        } catch (std::exception_ptr e) {
            return false;
        }
        is_inited_ = true;
        return true;
    }
    void shutdown()
    {
        spdlog::shutdown();
    }
    template <typename... Args>
    void log(const spdlog::source_loc &loc, spdlog::level::level_enum lvl, const char *fmt, const Args &... args)
    {
        spdlog::log(loc, lvl, fmt, args...);
    }
    template <typename... Args>
    void printf(const spdlog::source_loc &loc, spdlog::level::level_enum lvl, const char *fmt, const Args &... args)
    {
        spdlog::log(loc, lvl, fmt::sprintf(fmt, args...).c_str());
    }
    spdlog::level::level_enum level()
    {
        return log_level_;
    }
    void set_level(spdlog::level::level_enum lvl)
    {
        log_level_ = lvl;
        spdlog::set_level(log_level_);
    }
    void set_flush_on(spdlog::level::level_enum lvl)
    {
        spdlog::flush_on(lvl);
    }
    static const char *get_shortname(std::string_view path)
    {
        if (path.empty())
            return path.data();
        size_t pos = path.find_last_of("/\\");
        return path.data() + ((pos == path.npos) ? 0 : pos + 1);
    }
private:
    logger() = default;
    ~logger() = default;
    logger(const logger &) = delete;
    void operator=(const logger &) = delete;
private:
    std::atomic_bool is_inited_ = false;
    spdlog::level::level_enum log_level_ = spdlog::level::trace;
};

}

// 仅记录文件名
#define LOG_FILENAME (welog::logger::get_shortname(__FILE__))

#define LOG_LEVLE_TRACE spdlog::level::trace
#define LOG_LEVLE_DEBUG spdlog::level::debug
#define LOG_LEVLE_INFO  spdlog::level::info
#define LOG_LEVLE_WARN  spdlog::level::warn
#define LOG_LEVLE_ERROR spdlog::level::err
#define LOG_LEVLE_FATAL spdlog::level::critical

#define LOG_INIT(filename, level)                   \
{                                                   \
    if (welog::logger::get().init(filename)) {      \
        welog::logger::get().set_level(level);      \
        welog::logger::get().set_flush_on(level);   \
    }                                               \
}

/*
 * 示例
 * use fmt lib, e.g. LOG_WARN("warn log, {1}, {1}, {2}", 1, 2);
 */
#define LOG_TRACE(msg, ...) \
    spdlog::log({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::trace, msg, ##__VA_ARGS__)
#define LOG_DEBUG(msg, ...) \
    spdlog::log({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::debug, msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...) \
    spdlog::log({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::info, msg, ##__VA_ARGS__)
#define LOG_WARN(msg, ...) \
    spdlog::log({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::warn, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) \
    spdlog::log({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::err, msg, ##__VA_ARGS__)
#define LOG_FATAL(msg, ...) \
    spdlog::log({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::critical, msg, ##__VA_ARGS__)

/*
 * 示例
 * use like sprintf, e.g. PRINT_WARN("warn log, %d-%d", 1, 2);
 */
#define PRINT_TRACE(msg, ...) \
    welog::logger::get().printf({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::trace, msg, ##__VA_ARGS__)
#define PRINT_DEBUG(msg, ...) \
    welog::logger::get().printf({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::debug, msg, ##__VA_ARGS__)
#define PRINT_INFO(msg, ...) \
    welog::logger::get().printf({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::info, msg, ##__VA_ARGS__)
#define PRINT_WARN(msg, ...) \
    welog::logger::get().printf({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::warn, msg, ##__VA_ARGS__)
#define PRINT_ERROR(msg, ...) \
    welog::logger::get().printf({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::err, msg, ##__VA_ARGS__)
#define PRINT_FATAL(msg, ...) \
    welog::logger::get().printf({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::critical, msg, ##__VA_ARGS__)

/*
 * 示例
 * use like stream , e.g. STM_WARN() << "warn log: " << 1;
 */
#define STM_TRACE() \
    welog::logger::log_stream({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::trace, "")
#define STM_DEBUG() \
    welog::logger::log_stream({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::debug, "")
#define STM_INFO() \
    welog::logger::log_stream({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::info, "")
#define STM_WARN() \
    welog::logger::log_stream({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::warn, "")
#define STM_ERROR() \
    welog::logger::log_stream({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::err, "")
#define STM_FATAL() \
    welog::logger::log_stream({LOG_FILENAME, __LINE__, __FUNCTION__}, spdlog::level::critical, "")

#define LOG_ENTER()                                                                     \
struct __lambda__{                                                                      \
    __lambda__(const char *func) : m_func(func)                                         \
    {                                                                                   \
        spdlog::log({LOG_FILENAME, __LINE__, m_func}, spdlog::level::debug, "ENTER");   \
    }                                                                                   \
    ~__lambda__()                                                                       \
    {                                                                                   \
        spdlog::log({LOG_FILENAME, __LINE__, m_func}, spdlog::level::debug, "LEAVE");   \
    }                                                                                   \
    const char *m_func;                                                                 \
} __lambda_temp__(__FUNCTION__)

#endif
