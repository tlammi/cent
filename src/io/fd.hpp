#pragma once

#include <memory>
#include <utility>

namespace cent::io {

struct pidfd_t {};
constexpr pidfd_t pidfd{};

class Fd {
 public:
    constexpr Fd() noexcept = default;
    constexpr explicit Fd(int fd) noexcept : m_fd(fd) {}

    /**
     * \brief pidfd_open
     *
     * pid_t may be int so type cannot be used alone, alas, the tag
     * */
    explicit Fd(pidfd_t, pid_t pid);

    Fd(const Fd&) = delete;
    Fd& operator=(const Fd&) = delete;

    constexpr Fd(Fd&& other) noexcept : m_fd(std::exchange(other.m_fd, -1)) {}

    constexpr Fd& operator=(Fd&& other) noexcept {
        std::destroy_at(this);
        std::construct_at(this, std::move(other));
        return *this;
    }

    constexpr ~Fd() {
        if (m_fd < 0) return;
        ::close(m_fd);
    }

    constexpr int fd() const noexcept { return m_fd; }

    int release() noexcept { return std::exchange(m_fd, -1); }

    void close() noexcept {
        if (m_fd >= 0) ::close(std::exchange(m_fd, -1));
    }

 private:
    int m_fd{-1};
};

}  // namespace cent::io
