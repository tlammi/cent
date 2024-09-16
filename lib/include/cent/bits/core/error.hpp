#pragma once
#include <cent/bits/containers/enum_arr.hpp>
#include <cent/str.hpp>

namespace cent {

enum class ErrorCode {
    Perm,
};

constexpr auto ERROR_MSGS =
    make_enum_arr<ErrorCode, StaticStr>(ErrorCode::Perm, "permission denied");

class Error {
 public:
    constexpr Error(ErrorCode ec) noexcept : m_ec(ec), m_msg(ERROR_MSGS[ec]) {}
    constexpr Error(ErrorCode ec, const std::string& msg)
        : m_ec(ec), m_msg(msg) {}

 private:
    ErrorCode m_ec{};
    Str m_msg{};
};

}  // namespace cent
