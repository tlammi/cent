#pragma once

#include <coroutine>
#include <print>
#include <utility>

namespace cent::async {

template <class T>
class TaskPromise;

template <class T = void>
class Task {
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
        : m_hndl(std::move(p)) {}

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    constexpr Task(Task&& other) noexcept
        : m_hndl(std::exchange(other.m_hndl, {})) {}

    constexpr Task& operator=(Task&& other) noexcept {
        auto tmp = Task(std::move(other));
        std::swap(m_hndl, tmp.m_hndl);
        return *this;
    }

    constexpr ~Task() {
        if (m_hndl) m_hndl.destroy();
    }

    auto operator co_await() const& noexcept {
        struct Impl : public Awaitable {
            auto await_resume() -> decltype(auto) {
                if constexpr (!std::same_as<T, void>)
                    return this->m_h.promise().value();
            }
        };
        return Impl{m_hndl};
    }

    auto operator co_await() const&& noexcept {
        struct Impl : public Awaitable {
            auto await_resume() -> decltype(auto) {
                return std::move(this->m_h.promise()).result();
            }
        };
        return Impl{m_hndl};
    }

    constexpr auto done() const noexcept { return m_hndl.done(); }
    constexpr void resume() const { m_hndl.resume(); }

    constexpr auto& promise() const { return m_hndl.promise(); }

 private:
    coro_handle m_hndl{};
};

template <class T>
constexpr T run(Task<T>& t) {
    while (!t.done()) t.resume();
    if constexpr (!std::same_as<T, void>) { return t.promise().value(); }
}

}  // namespace cent::async
