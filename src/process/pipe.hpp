#pragma once

#include <limits.h>

#include <span>
#include <utility>

namespace cent::process {

constexpr size_t MAX_PACKET = PIPE_BUF - 1;

namespace detail {

class PipeImpl {
 public:
    PipeImpl(const PipeImpl&) = delete;
    PipeImpl& operator=(const PipeImpl&) = delete;
    PipeImpl(PipeImpl&& other) noexcept : m_fd(std::exchange(other.m_fd, 0)) {}
    PipeImpl& operator=(PipeImpl&& other) {
        std::destroy_at(this);
        std::construct_at(this, std::move(other));
        return *this;
    }
    ~PipeImpl();

 protected:
    PipeImpl(int fd) noexcept : m_fd(fd) {}
    void write(std::span<const std::byte> data);
    void read(std::span<std::byte> data);

 private:
    int m_fd;
};

std::pair<int, int> open_pipe(bool packet_mode);

}  // namespace detail
template <class T>
class PipeIn : detail::PipeImpl {
    using Parent = detail::PipeImpl;

 public:
    explicit PipeIn(int fd) : Parent(fd) {}
    PipeIn& operator>>(T& out) {
        Parent::read(
            std::span(reinterpret_cast<std::byte*>(std::addressof(out)),
                      sizeof(std::remove_cvref_t<T>)));
        return *this;
    }
};

template <class T>
class PipeOut : detail::PipeImpl {
    using Parent = detail::PipeImpl;

 public:
    explicit PipeOut(int fd) : Parent(fd) {}
    PipeOut& operator<<(const T& in) {
        Parent::write(
            std::span(reinterpret_cast<const std::byte*>(std::addressof(in)),
                      sizeof(std::remove_cvref_t<T>)));
        return *this;
    }
};

template <class T>
auto pipe() {
    auto [r, w] = detail::open_pipe(true);
    return std::pair{PipeIn<T>(r), PipeOut<T>(w)};
}

}  // namespace cent::process
