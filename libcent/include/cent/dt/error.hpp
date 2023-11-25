#pragma once

#include <cent/dt/errno.hpp>
#include <string>

namespace cent {

class Error {
 public:
    constexpr Error(Errno e) noexcept : m_e(e) {}

    std::string_view message() noexcept {
        if (m_s.empty()) return errno_message(m_e).view();
        return m_s;
    }
    constexpr Errno code() const noexcept { return m_e; }

 private:
    Errno m_e;
    std::string m_s{};
};
}  // namespace cent
