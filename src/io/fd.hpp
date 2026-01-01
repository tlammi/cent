#pragma once

#include <memory>
#include <utility>

namespace cent::io {

class Fd {
 public:
    constexpr Fd() noexcept = default;
    constexpr explicit Fd(int fd) noexcept : m_fd(fd) {}

    Fd(const Fd&) = delete;
    Fd& operator=(const Fd&) = delete;

    Fd(Fd&& other) noexcept : m_fd(std::exchange(other.m_fd, 0)) {}

    Fd& operator=(Fd&& other) noexcept {
        std::destroy_at(this);
        std::construct_at(this, std::move(other));
        return *this;
    }

    constexpr ~Fd() {
        if (!m_fd) return;
        ::close(m_fd);
    }

    int fd() const noexcept { return m_fd; }

    int release() noexcept { return std::exchange(m_fd, 0); }

    void close() noexcept {
        if (m_fd) ::close(std::exchange(m_fd, 0));
    }

 private:
    int m_fd;
};

}  // namespace cent::io
