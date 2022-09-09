#pragma once

#include <sstream>

#include "cent/log_level.hpp"

namespace cent::logs {
namespace detail {

struct LogMsg {
    LogLevel log_level;
    std::stringstream ss;
};

void push_log(LogMsg&& msg);

void do_print(std::stringbuf* buf);

template <class T, class... Ts>
void log_recurse(std::stringstream& ss, T&& t, Ts&&... ts) {
    ss << std::forward<T>(t);
    if constexpr (sizeof...(Ts)) log_recurse(ss, std::forward<Ts>(ts)...);
}

template <class... Ts>
void do_log(LogLevel log_level, Ts&&... ts) {
    LogMsg msg{log_level, std::stringstream{}};
    log_recurse(msg.ss, std::forward<Ts>(ts)...);
    push_log(std::move(msg));
}

}  // namespace detail

template <class... Ts>
constexpr void trace(Ts&&... ts) {
    detail::do_log(LogLevel::Trace, std::forward<Ts>(ts)...);
};

template <class... Ts>
constexpr void debug(Ts&&... ts) {
    detail::do_log(LogLevel::Debug, std::forward<Ts>(ts)...);
}

template <class... Ts>
constexpr void info(Ts&&... ts) {
    detail::do_log(LogLevel::Info, std::forward<Ts>(ts)...);
}

template <class... Ts>
constexpr void note(Ts&&... ts) {
    detail::do_log(LogLevel::Note, std::forward<Ts>(ts)...);
}

template <class... Ts>
constexpr void warn(Ts&&... ts) {
    detail::do_log(LogLevel::Warn, std::forward<Ts>(ts)...);
}

template <class... Ts>
constexpr void err(Ts&&... ts) {
    detail::do_log(LogLevel::Err, std::forward<Ts>(ts)...);
}

template <class... Ts>
constexpr void fatal(Ts&&... ts) {
    detail::do_log(LogLevel::Fatal, std::forward<Ts>(ts)...);
}

template <class... Ts>
void print(Ts&&... ts) {
    std::stringstream ss;
    detail::log_recurse(ss, std::forward<Ts>(ts)...);
    detail::do_print(ss.rdbuf());
}

}  // namespace cent::logs