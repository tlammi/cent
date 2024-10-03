#pragma once

#include <cent/bits/async/detail/exec_ctx.hpp>
#include <cent/util.hpp>
#include <coroutine>
#include <exception>
#include <print>
#include <utility>
#include <variant>

namespace cent::async {
namespace task_detail {
struct unused {};
}  // namespace task_detail

class Executor;

template <class T>
class TaskPromise;

template <class T = void>
class Task {
    friend class Executor;

 public:
    using promise_type = TaskPromise<T>;
    using value_type = T;
    using coro_handle = std::coroutine_handle<promise_type>;

    class Awaitable {
     public:
        constexpr Awaitable(coro_handle h) noexcept : m_h(std::move(h)) {}
        auto await_ready() const noexcept -> bool { return !m_h || m_h.done(); }
        template <class C>
        void await_suspend(
            std::coroutine_handle<C> awaiting_coroutine) noexcept {
            auto* exec = awaiting_coroutine.promise().executor();
            CENT_ASSERT(exec);
            exec->current_stack().push(m_h);
            m_h.promise().executor(exec);
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

    ~Task() {
        if (m_h) m_h.destroy();
    }

    auto operator co_await() const& noexcept {
        struct Impl : public Awaitable {
            auto await_resume() -> decltype(auto) {
                std::println("await_resume");
                this->m_h.promise().rethrow();
                return this->m_h.promise().value();
            }
        };
        return Impl{m_h};
    }

    auto operator co_await() const&& noexcept {
        struct Impl : public Awaitable {
            auto await_resume() -> decltype(auto) {
                std::println("await_resume");
                this->m_h.promise().rethrow();
                return std::move(this->m_h.promise()).value();
            }
        };
        return Impl{m_h};
    }

    constexpr auto done() const noexcept { return m_h.done(); }
    constexpr void resume() const { m_h.resume(); }

    constexpr auto& promise() const noexcept { return m_h.promise(); }
    constexpr auto handle() const noexcept { return m_h; }

 private:
    std::coroutine_handle<TaskPromise<T>> m_h{};
};

template <class T>
class TaskPromiseBase {
 public:
    friend struct FinalAvaitable;
    struct FinalAvaitable {
        constexpr auto await_ready() const noexcept { return false; }

        template <class P>
        constexpr void await_suspend(
            std::coroutine_handle<P> coro) const noexcept {}

        constexpr auto await_resume() noexcept {}
    };
    constexpr auto get_return_object() {
        return typename T::task_type{
            std::coroutine_handle<T>::from_promise(*static_cast<T*>(this))};
    }
    constexpr std::suspend_always initial_suspend() const noexcept {
        return {};
    }

    constexpr auto final_suspend() const noexcept {
        std::println("final suspend");
        return FinalAvaitable();
    }

    constexpr detail::ExecCtx* executor() const noexcept { return m_exec; }
    constexpr void executor(detail::ExecCtx* exec) noexcept { m_exec = exec; }

 protected:
    constexpr ~TaskPromiseBase() = default;

 private:
    detail::ExecCtx* m_exec{};
};

template <class T>
class TaskPromise : public TaskPromiseBase<TaskPromise<T>> {
 public:
    using task_type = Task<T>;
    template <class V>
    constexpr void return_value(V&& v) noexcept {
        m_val.template emplace<T>(std::forward<V>(v));
    }

    template <class Self>
    decltype(auto) value(this Self&& self) noexcept {
        return std::get<T>(std::forward<Self>(self).m_val);
    }

    void unhandled_exception() noexcept {
        m_val.template emplace<std::exception_ptr>(std::current_exception());
    }

    void rethrow() const {
        if (std::holds_alternative<std::exception_ptr>(m_val)) {
            std::rethrow_exception(std::get<std::exception_ptr>(m_val));
        }
    }

 private:
    std::variant<task_detail::unused, T, std::exception_ptr> m_val{};
};

template <>
class TaskPromise<void> : public TaskPromiseBase<TaskPromise<void>> {
 public:
    using task_type = Task<void>;
    constexpr void return_void() const noexcept { std::println("return void"); }

    void unhandled_exception() noexcept {
        std::println("set exception");
        m_val.template emplace<std::exception_ptr>(std::current_exception());
    }

    void rethrow() const {
        std::println("check rethrow");
        if (std::holds_alternative<std::exception_ptr>(m_val)) {
            std::rethrow_exception(std::get<std::exception_ptr>(m_val));
        }
        std::println("did not throw");
    }

    void value() const noexcept {}

 private:
    std::variant<task_detail::unused, std::exception_ptr> m_val{};
};

}  // namespace cent::async
