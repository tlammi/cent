#pragma once

#include <format>
#include <string_view>

#include "cent/cast.hpp"
#include "cent/int.hpp"

namespace cent::logs {

enum class Level : u8 {
    Trace = 0,
    Debug,
    Info,
    Note,
    Warn,
    Err,
    Fatal,
    None,
};

constexpr auto operator<=>(Level a, Level b) noexcept {
    return underlying_cast(a) <=> underlying_cast(b);
}

void init(Level lvl, void (*on_log)(Level lvl, std::string_view msg));
void set_level(Level lvl) noexcept;
Level level() noexcept;

namespace detail {
class Logger {
 public:
    consteval explicit Logger(Level lvl) noexcept : m_lvl(lvl) {}
    template <class... Ts>
    void operator()(std::format_string<Ts...> fmt, Ts&&... ts) const {
        if (m_lvl < level()) return;
        const auto msg = std::format(fmt, std::forward<Ts>(ts)...);
        do_log(msg);
    }

    void operator()(std::string_view msg) const {
        if (m_lvl < level()) return;
        do_log(msg);
    }

 private:
    void do_log(std::string_view s) const;
    Level m_lvl;
};
}  // namespace detail

constexpr auto TRACE = detail::Logger(Level::Trace);
constexpr auto DEBUG = detail::Logger(Level::Debug);
constexpr auto INFO = detail::Logger(Level::Info);
constexpr auto NOTE = detail::Logger(Level::Note);
constexpr auto WARN = detail::Logger(Level::Warn);
constexpr auto ERR = detail::Logger(Level::Err);
constexpr auto FATAL = detail::Logger(Level::Fatal);

}  // namespace cent::logs
