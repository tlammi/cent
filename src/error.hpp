#pragma once

#include <format>

#include "const_str.hpp"
#include "exception.hpp"
#include "static_str.hpp"

namespace cent {

enum class ErrorCode {
    Generic,
    Internal,
    InvalidArgument,
    FormatError,
    MissingArgument,
    DoesNotExist,
};

class Error {
 public:
    constexpr Error(ErrorCode ec) : m_ec(ec) {}
    constexpr Error(ErrorCode ec, ConstStr what) noexcept
        : m_ec(ec), m_what(std::move(what)) {}

    constexpr ErrorCode code() const noexcept { return m_ec; }
    constexpr const char* what() const noexcept { return m_what.c_str(); }

    [[noreturn]] void raise() && { throw Exception(std::move(*this)); }

 private:
    ErrorCode m_ec;
    ConstStr m_what{};
};

inline auto make_error(ErrorCode ec, StaticStr s) {
    return Error(ec, ConstStr(s));
}

[[noreturn]] inline void raise(ErrorCode ec, StaticStr s) {
    Error(ec, ConstStr(s)).raise();
}

[[noreturn]] inline void raise_errno() {
    Error(ErrorCode::Generic, ConstStr(strerror(errno))).raise();
}

template <class... Ts>
    requires(sizeof...(Ts) > 0)
Error make_error(ErrorCode ec, std::format_string<Ts...> fmt, Ts&&... ts) {
    return Error(ec, ConstStr(std::format(fmt, std::forward<Ts>(ts)...)));
}

template <class... Ts>
    requires(sizeof...(Ts) > 0)
[[noreturn]] void raise(ErrorCode ec, std::format_string<Ts...> fmt,
                        Ts&&... ts) {
    Error(ec, ConstStr(std::format(fmt, std::forward<Ts>(ts)...))).raise();
}

}  // namespace cent
