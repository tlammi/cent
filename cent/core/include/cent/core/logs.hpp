#pragma once

#include <fmt/format.h>

#include <cent/core/util.hpp>
#include <cstdint>
#include <memory>

namespace cent {

enum class LogLevel : std::uint8_t {
    Trace,
    Debug,
    Info,
    Warn,
    Err,
};

void init(LogLevel lvl);
LogLevel level();

void flush();

namespace logdetail {

void push_log(LogLevel lvl, std::string&& msg);

template <class Fmt, class... Ts>
void do_log(LogLevel lvl, Fmt&& fmt, Ts&&... ts) {
    push_log(lvl, fmt::format(std::forward<Fmt>(fmt), std::forward<Ts>(ts)...));
}

}  // namespace logdetail

// NOLINTNEXTLINE
#define LOG_DO_LOG(lvl, ...)                                   \
    do {                                                       \
        if (underlying_cast(lvl) < underlying_cast(level())) { \
            ::cent::logdetail::do_log(lvl, __VA_ARGS__)        \
        }                                                      \
    } while (0)

// NOLINTNEXTLINE
#define LOG_TRACE(...) LOG_DO_LOG(::cent::LogLevel::Trace, __VA_ARGS__)
// NOLINTNEXTLINE
#define LOG_DEBUG(...) LOG_DO_LOG(::cent::LogLevel::Debug, __VA_ARGS__)
// NOLINTNEXTLINE
#define LOG_INFO(...) LOG_DO_LOG(::cent::LogLevel::Info, __VA_ARGS__)
// NOLINTNEXTLINE
#define LOG_WARN(...) LOG_DO_LOG(::cent::LogLevel::Warn, __VA_ARGS__)
// NOLINTNEXTLINE
#define LOG_ERR(...) LOG_DO_LOG(::cent::LogLevel::Err, __VA_ARGS__)

}  // namespace cent
