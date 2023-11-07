#pragma once

#include <fmt/format.h>

#include <string>

namespace cent::core::logs {

enum class Level {
    Trace,
    Debug,
    Info,
    Note,
    Warn,
    Err,
    Fatal,
};

void init(void (*consumer)(Level, std::string));
void set_level(Level level);
Level active_level();

namespace detail {

void do_push_log(Level lvl, std::string s);
template <class... Ts>
void push_log(Level lvl, fmt::format_string<Ts...> fmt, Ts&&... ts) {
    do_push_log(lvl, fmt::format(fmt, std::forward<Ts>(ts)...));
}

}  // namespace detail

// NOLINTNEXTLINE
#define CENT_DETAIL_DO_LOG(lvl, ...)               \
    if (lvl >= ::cent::core::logs::active_level()) \
    ::cent::core::logs::detail::push_log(lvl, __VA_ARGS__)

// NOLINTBEGIN
#define CENT_TRACE(...) \
    CENT_DETAIL_DO_LOG(::cent::core::logs::Level::Trace, __VA_ARGS__)
#define CENT_DEBUG(...) \
    CENT_DETAIL_DO_LOG(::cent::core::logs::Level::Debug, __VA_ARGS__)
#define CENT_INFO(...) \
    CENT_DETAIL_DO_LOG(::cent::core::logs::Level::Info, __VA_ARGS__)
#define CENT_NOTE(...) \
    CENT_DETAIL_DO_LOG(::cent::core::logs::Level::Note, __VA_ARGS__)
#define CENT_WARN(...) \
    CENT_DETAIL_DO_LOG(::cent::core::logs::Level::Warn, __VA_ARGS__)
#define CENT_ERR(...) \
    CENT_DETAIL_DO_LOG(::cent::core::logs::Level::Error, __VA_ARGS__)
#define CENT_FATAL(...) \
    CENT_DETAIL_DO_LOG(::cent::core::logs::Level::Fatal, __VA_ARGS__)
// NOLINTEND

}  // namespace cent::core::logs
