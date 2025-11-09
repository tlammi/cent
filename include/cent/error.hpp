#pragma once

#include <cent/const_str.hpp>
#include <cent/exception.hpp>
#include <cent/static_str.hpp>

namespace cent {

enum class ErrorCode {
    Generic,
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

[[noreturn]] void raise(ErrorCode ec, StaticStr s) {
    Error(ec, ConstStr(s)).raise();
}

}  // namespace cent
