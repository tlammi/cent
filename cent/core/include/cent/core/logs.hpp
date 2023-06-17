#pragma once

#include <fmt/format.h>

#include <cent/core/util.hpp>
#include <cstdint>
#include <memory>
#include <stdexcept>

namespace cent {

enum class LogLevel : std::uint8_t {
    Trace,
    Debug,
    Info,
    Warn,
    Err,
};

constexpr LogLevel to_log_level(std::string_view s) {
    using enum LogLevel;
    if (s == "trace") return Trace;
    if (s == "debug") return Debug;
    if (s == "info") return Info;
    if (s == "warn") return Warn;
    if (s == "err") return Err;
    throw std::runtime_error("Invalid log level");
}

void logs_init(LogLevel lvl);
LogLevel log_level();

void logs_flush();

namespace logdetail {

void push_log(LogLevel lvl, std::string&& msg);

template <class... Ts>
void do_log(LogLevel lvl, fmt::format_string<Ts&&...> fmtstr, Ts&&... ts) {
    push_log(lvl, fmt::format(fmtstr, std::forward<Ts>(ts)...));
}

}  // namespace logdetail

// NOLINTNEXTLINE
#define LOG_DO_LOG(lvl, ...)                                \
    do {                                                    \
        if (::cent::underlying_cast(lvl) <=                 \
            ::cent::underlying_cast(::cent::log_level())) { \
            ::cent::logdetail::do_log(lvl, __VA_ARGS__);    \
        }                                                   \
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

// NOLINTNEXTLINE
#define LOG_EXPR(expr)                \
    [&] {                             \
        LOG_TRACE("expr: {}", #expr); \
        auto res = expr;              \
        LOG_TRACE("  -> {}", res);    \
        return res;                   \
    }()

// NOLINTNEXTLINE
#define LOG_STMT(stmt)          \
    [&] {                       \
        LOG_TRACE("{}", #stmt); \
        stmt;                   \
    }()
}  // namespace cent
