#pragma once

#include <sstream>

namespace cent::logs {
namespace detail {
struct LogMsg {
    bool is_debug;
    std::stringstream ss;
};

void push_log(LogMsg&& msg);

template <class T, class... Ts>
void log_recurse(std::stringstream& ss, T&& t, Ts&&... ts) {
    ss << std::forward<T>(t);
    if constexpr (sizeof...(Ts)) log_recurse(ss, std::forward<Ts>(ts)...);
}

template <class... Ts>
void do_log(bool is_debug, Ts&&... ts) {
    LogMsg msg{is_debug, std::stringstream{}};
    log_recurse(msg.ss, std::forward<Ts>(ts)...);
    push_log(std::move(msg));
}
}  // namespace detail

template <class... Ts>
void debug(Ts&&... ts) {
    detail::do_log(true, std::forward<Ts>(ts)...);
}

template <class... Ts>
void info(Ts&&... ts) {
    detail::do_log(false, std::forward<Ts>(ts)...);
}

}  // namespace cent::logs