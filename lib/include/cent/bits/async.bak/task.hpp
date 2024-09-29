#pragma once

#include <cent/bits/async/detail/handle.hpp>
#include <coroutine>
#include <print>
#include <utility>

namespace cent::async {
namespace detail {
class RootTask;
}

template <class T>
class TaskPromise;

template <class T = void>
class Task {
    friend class detail::RootTask;

 public:
    using promise_type = TaskPromise<T>;
    using value_type = T;
    using coro_handle = std::coroutine_handle<promise_type>;

    class Awaitable {
     public:
        constexpr Awaitable(coro_handle h) noexcept : m_h(std::move(h)) {}
        auto await_ready() const noexcept -> bool { return !m_h || m_h.done(); }
        auto await_suspend(std::coroutine_handle<> awaiting_coroutine) noexcept
            -> std::coroutine_handle<> {
            m_h.promise().continuation(awaiting_coroutine);
            return m_h;
        }

     protected:
        coro_handle m_h;  // NOLINT
    };
    constexpr Task() noexcept = default;
    constexpr explicit Task(std::coroutine_handle<TaskPromise<T>> p)
        : m_h(std::move(p)) {}

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    constexpr Task(Task&& other) noexcept : m_h(std::exchange(other.m_h, {})) {}

    constexpr Task& operator=(Task&& other) noexcept {
        auto tmp = Task(std::move(other));
        std::swap(m_h, tmp.m_h);
        return *this;
    }

    auto operator co_await() const& noexcept {
        struct Impl : public Awaitable {
            auto await_resume() -> decltype(auto) {
                if constexpr (!std::same_as<T, void>)
                    return this->m_h.promise().value();
            }
        };
        return Impl{m_h.handle()};
    }

    auto operator co_await() const&& noexcept {
        struct Impl : public Awaitable {
            auto await_resume() -> decltype(auto) {
                return std::move(this->m_h.promise()).result();
            }
        };
        return Impl{m_h.handle()};
    }

    constexpr auto done() const noexcept { return m_h.done(); }
    constexpr void resume() const { m_h.resume(); }

    constexpr auto& promise() const { return m_h.promise(); }

 private:
    detail::Handle<TaskPromise<T>> m_h{};
};

template <class T>
constexpr T run(Task<T>& t) {
    while (!t.done()) t.resume();
    if constexpr (!std::same_as<T, void>) { return t.promise().value(); }
}

template <class T>
Task<> to_void_task(T* out, Task<T> t) {
    *out = co_await t;
}
}  // namespace cent::async
