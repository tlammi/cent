#pragma once

#include <coroutine>
#include <utility>

namespace cent::async::detail {

template <class P>
class Handle {
 public:
    using promise_type = P;
    using handle_type = std::coroutine_handle<P>;

    constexpr Handle() noexcept = default;

    constexpr Handle(std::nullptr_t /*unused*/) noexcept : Handle() {}

    constexpr explicit Handle(handle_type h) noexcept : m_h(h) {}

    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;

    constexpr Handle(Handle&& other) noexcept
        : m_h(std::exchange(other.m_h, {})) {}

    constexpr Handle& operator=(Handle&& other) noexcept {
        auto tmp = Handle(std::move(other));
        std::swap(m_h, tmp.m_h);
        return *this;
    }

    constexpr ~Handle() {
        if (m_h) m_h.destroy();
    }

    auto& promise() const { return m_h.promise(); }
    bool done() const { return m_h.done(); }

    void resume() const { return m_h.resume(); }

    void destroy() const { m_h.destroy(); }

    template <class S>
    decltype(auto) raw(this S&& s) noexcept {
        return std::forward<S>(s).m_h;
    }

 private:
    handle_type m_h{};
};

template <class P>
class WeakHandle {
 public:
    using promise_type = P;
    using handle_type = std::coroutine_handle<P>;

    constexpr WeakHandle() noexcept = default;

    constexpr WeakHandle(std::nullptr_t /*unused*/) noexcept : WeakHandle() {}
    constexpr WeakHandle(handle_type h) noexcept : m_h(h) {}

    constexpr WeakHandle(const Handle<P>& h) noexcept : m_h(h.raw()) {}

    WeakHandle(const WeakHandle&) noexcept = default;
    WeakHandle& operator=(const WeakHandle&) noexcept = default;

    WeakHandle(WeakHandle&&) noexcept = default;
    WeakHandle& operator=(WeakHandle&&) noexcept = default;

    constexpr ~WeakHandle() = default;

    auto& promise() const { return m_h.promise(); }
    bool done() const { return m_h.done(); }

    void resume() const { return m_h.resume(); }

    void destroy() const { m_h.destroy(); }

    template <class S>
    decltype(auto) raw(this S&& s) noexcept {
        return std::forward<S>(s).m_h;
    }

 private:
    handle_type m_h{};
};

}  // namespace cent::async::detail
