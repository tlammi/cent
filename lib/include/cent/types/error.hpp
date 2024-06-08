#pragma once

#include <cent/str.hpp>

namespace cent {
namespace error_detail {
#include <cent/types/errno.gen.inl>
}

using ErrorCode = error_detail::Errno;

class Error {
 public:
    constexpr explicit Error(int e, str::StaticStr msg = "")
        : m_code(ErrorCode{e}),
          m_msg(msg.empty() ? error_detail::errno_to_desc(m_code) : msg) {}

    constexpr ErrorCode code() const noexcept { return m_code; }
    constexpr str::CStr message() const noexcept { return m_msg.view(); }

 private:
    ErrorCode m_code{};
    str::ConstStr m_msg{};
};

}  // namespace cent
