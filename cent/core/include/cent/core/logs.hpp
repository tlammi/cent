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
#define CENT_DO_LOG(lvl, ...)                               \
    do {                                                    \
        if (::cent::underlying_cast(lvl) >=                 \
            ::cent::underlying_cast(::cent::log_level())) { \
            ::cent::logdetail::do_log(lvl, __VA_ARGS__);    \
        }                                                   \
    } while (0)

// NOLINTNEXTLINE
#define CENT_TRACE(...) CENT_DO_LOG(::cent::LogLevel::Trace, __VA_ARGS__)
// NOLINTNEXTLINE
#define CENT_DEBUG(...) CENT_DO_LOG(::cent::LogLevel::Debug, __VA_ARGS__)
// NOLINTNEXTLINE
#define CENT_INFO(...) CENT_DO_LOG(::cent::LogLevel::Info, __VA_ARGS__)
// NOLINTNEXTLINE
#define CENT_WARN(...) CENT_DO_LOG(::cent::LogLevel::Warn, __VA_ARGS__)
// NOLINTNEXTLINE
#define CENT_ERR(...) CENT_DO_LOG(::cent::LogLevel::Err, __VA_ARGS__)

// NOLINTNEXTLINE
#define CENT_EXPR(expr)                \
    [&] {                              \
        CENT_TRACE("expr: {}", #expr); \
        auto res = expr;               \
        CENT_TRACE("  -> {}", res);    \
        return res;                    \
    }()

/**
 * NOTE: Careful with this and if blocks without braces
 * */
// NOLINTNEXTLINE
#define CENT_STMT(stmt)      \
    CENT_TRACE("{}", #stmt); \
    stmt;
}  // namespace cent

