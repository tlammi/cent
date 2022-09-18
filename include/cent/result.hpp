/* SPDX-License-Identifier:  GPL-3.0-or-later */
#pragma once

#include <ostream>
#include <string>

namespace cent {

/**
 * Libcent result
 *
 * Contains a status code and possible error message.
 */
class Result {
 public:
    /**
     * Create result with code and message
     */
    Result(int code, std::string msg) : m_code{code}, m_msg{std::move(msg)} {}

    Result(const std::error_code& ec)
        : m_code{ec.value()}, m_msg{ec.message()} {}

    // alias for ok()
    explicit operator bool() const noexcept { return !m_code; }
    // True if error code is 0
    bool ok() const noexcept { return !m_code; }

    // Error code
    int code() const noexcept { return m_code; }

    // message
    std::string_view msg() const noexcept { return m_msg; }

 private:
    int m_code{};
    std::string m_msg{};
};

inline std::ostream& operator<<(std::ostream& os, const Result& r) {
    os << '(' << r.code() << ", " << r.msg() << ')';
    return os;
}

}  // namespace cent