#pragma once

#include <map>
#include <string>

namespace cent::net::http {

class StatusCode {
 public:
    constexpr StatusCode() noexcept = default;
    constexpr StatusCode(int i) noexcept : m_val(i) {}

    [[nodiscard]] constexpr bool info() const noexcept {
        return m_val >= 100 && m_val < 200;
    }

    [[nodiscard]] constexpr bool success() const noexcept {
        return m_val >= 200 && m_val < 300;
    }

    [[nodiscard]] constexpr bool redirect() const noexcept {
        return m_val >= 300 && m_val < 400;
    }
    [[nodiscard]] constexpr bool client_error() const noexcept {
        return m_val >= 400 && m_val < 500;
    }
    [[nodiscard]] constexpr bool server_error() const noexcept {
        return m_val >= 500 && m_val < 600;
    }

    constexpr operator int() const noexcept { return m_val; }

 private:
    int m_val{};
};
class Response {
 public:
    std::string text();
    const std::map<std::string, std::string>& headers();

 private:
};
}  // namespace cent::net::http
