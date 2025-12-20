#pragma once

#include <limits.h>

#include <span>
#include <utility>
#include <vector>

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

    void close();

 protected:
    PipeImpl(int fd) noexcept : m_fd(fd) {}
    void write(std::span<const std::byte> data);
    size_t read(std::span<std::byte> data);

 private:
    int m_fd;
};

std::pair<int, int> open_pipe(bool packet_mode);

}  // namespace detail
template <class T>
class PipeIn : detail::PipeImpl {
    static_assert(std::is_trivially_copyable_v<T>);
    using Parent = detail::PipeImpl;

 public:
    using Parent::close;

    explicit PipeIn(int fd) : Parent(fd) {}

    PipeIn& operator>>(T& out) {
        Parent::read(
            std::span(reinterpret_cast<std::byte*>(std::addressof(out)),
                      sizeof(std::remove_cvref_t<T>)));
        return *this;
    }
};

template <class T>
class PipeIn<T[]> : detail::PipeImpl {
    static_assert(std::is_trivially_copyable_v<T>);
    using Parent = detail::PipeImpl;

 public:
    using Parent::close;

    explicit PipeIn(int fd) : Parent(fd) {}

    PipeIn& operator>>(std::vector<T>& out) {
        auto raw_span = std::span<std::byte>(
            reinterpret_cast<std::byte*>(out.data()), out.size() * sizeof(T));
        auto count = Parent::read(raw_span);
        out.resize(count / sizeof(T));
        return *this;
    }
};

template <class T>
class PipeOut : detail::PipeImpl {
    static_assert(std::is_trivially_copyable_v<T>);
    using Parent = detail::PipeImpl;

 public:
    using Parent::close;

    explicit PipeOut(int fd) : Parent(fd) {}
    PipeOut& operator<<(const T& in) {
        Parent::write(
            std::span(reinterpret_cast<const std::byte*>(std::addressof(in)),
                      sizeof(std::remove_cvref_t<T>)));
        return *this;
    }
};

template <class T>
class PipeOut<T[]> : detail::PipeImpl {
    static_assert(std::is_trivially_copyable_v<T>);
    using Parent = detail::PipeImpl;

 public:
    using Parent::close;

    explicit PipeOut(int fd) : Parent(fd) {}

    PipeOut& operator<<(std::span<const T> in) {
        auto raw_span = std::span<const std::byte>(
            reinterpret_cast<const std::byte*>(in.data()),
            in.size() * sizeof(T));
        Parent::write(raw_span);
        return *this;
    }
};

template <class T>
auto pipe() {
    auto [r, w] = detail::open_pipe(true);
    return std::pair{PipeIn<T>(r), PipeOut<T>(w)};
}

}  // namespace cent::process
