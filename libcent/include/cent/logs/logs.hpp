#pragma once

#include <fmt/format.h>

#include <cent/logs/level.hpp>

#if !defined(CENT_MIN_LOG_LEVEL)
#define CENT_MIN_LOG_LEVEL Trace
#endif

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

namespace cent::logs {

struct Message {
    std::string msg;
    Level lvl;
};
class Logger {
 public:
    virtual ~Logger() = default;
    virtual void log(const Message& msg) = 0;
};

namespace detail {

void push_log(Level lvl, std::string msg);

consteval Level min_log_level() {
    using enum Level;
    return CENT_MIN_LOG_LEVEL;
}
}  // namespace detail

void init(Level lvl, std::unique_ptr<Logger> logger);
Level level();

}  // namespace cent::logs

#define CENT_DETAIL_LOG(lvl, ...)                                        \
    do {                                                                 \
        if constexpr (::cent::logs::detail::min_log_level() <=           \
                      ::cent::logs::Level::lvl)                          \
            if (level() <= ::cent::logs::Level::lvl)                     \
                ::cent::logs::detail::push_log(::cent::logs::Level::lvl, \
                                               fmt::format(__VA_ARGS__)) \
    } while (0)

#define CENT_TRACE(...) CENT_DETAIL_LOG(Trace, __VA_ARGS__)
#define CENT_DEBUG(...) CENT_DETAIL_LOG(Debug, __VA_ARGS__)
#define CENT_INFO(...)  CENT_DETAIL_LOG(Info, __VA_ARGS__)
#define CENT_NOTE(...)  CENT_DETAIL_LOG(Note, __VA_ARGS__)
#define CENT_WARN(...)  CENT_DETAIL_LOG(Warn, __VA_ARGS__)
#define CENT_ERR(...)   CENT_DETAIL_LOG(Err, __VA_ARGS__)
#define CENT_FATAL(...) CENT_DETAIL_LOG(Fatal, __VA_ARGS__)

// NOLINTEND(cppcoreguidelines-macro-usage)
