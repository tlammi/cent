#pragma once

#include <cent/str.hpp>

namespace cent {
namespace error_detail {
#include <cent/types/errno.gen.inl>
}

using ErrorCode = error_detail::Errno;

class Error {
 public:
    constexpr Error(int e)
        : m_code(ErrorCode(e)), m_msg(error_detail::errno_to_desc(m_code)) {}

    template <size_t S>
    consteval Error(int e, const char (&arr)[S]) noexcept
        : Error(e, str::StaticStr(arr)) {}

    constexpr explicit Error(int e, str::StaticStr msg) noexcept
        : m_code(ErrorCode{e}), m_msg(msg) {}

    constexpr explicit Error(int e, const std::string& msg)
        : m_code(ErrorCode{e}), m_msg(msg) {}

    constexpr ErrorCode code() const noexcept { return m_code; }
    constexpr str::CStr message() const noexcept { return m_msg.view(); }

 private:
    ErrorCode m_code{};
    str::ConstStr m_msg{};
};

}  // namespace cent
