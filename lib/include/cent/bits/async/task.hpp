#pragma once

#include <coroutine>
#include <exception>
#include <optional>
#include <utility>

namespace cent::async {

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

    ~Task() {
        if (m_h) m_h.destroy();
    }

    auto operator co_await() const& noexcept {
        struct Impl : public Awaitable {
            auto await_resume() -> decltype(auto) {
                if constexpr (!std::same_as<T, void>)
                    return this->m_h.promise().value();
            }
        };
        return Impl{m_h};
    }

    auto operator co_await() const&& noexcept {
        struct Impl : public Awaitable {
            auto await_resume() -> decltype(auto) {
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
        constexpr std::coroutine_handle<> await_suspend(
            std::coroutine_handle<P> coro) noexcept {
            auto& p = coro.promise();
            if (p.m_cont) { return p.m_cont; }
            return std::noop_coroutine();
        }

        constexpr auto await_resume() noexcept {}
    };
    constexpr auto get_return_object() {
        return typename T::task_type{
            std::coroutine_handle<T>::from_promise(*static_cast<T*>(this))};
    }
    constexpr std::suspend_always initial_suspend() const noexcept {
        return {};
    }

    constexpr auto final_suspend() const noexcept { return FinalAvaitable(); }

    void unhandled_exception() const noexcept { std::terminate(); }

    constexpr void continuation(std::coroutine_handle<> cont) noexcept {
        m_cont = cont;
    }

    constexpr Executor* executor() const noexcept { return m_exec; }
    constexpr void executor(Executor* exec) noexcept { m_exec = exec; }

 protected:
    constexpr ~TaskPromiseBase() = default;

 private:
    std::coroutine_handle<> m_cont{};
    Executor* m_exec{};
};

template <class T>
class TaskPromise : public TaskPromiseBase<TaskPromise<T>> {
 public:
    using task_type = Task<T>;
    template <class V>
    constexpr void return_value(V&& v) noexcept {
        m_val.emplace(std::forward<V>(v));
    }

    template <class Self>
    decltype(auto) value(this Self&& self) noexcept {
        return *(std::forward<Self>(self).m_val);
    }

 private:
    std::optional<T> m_val{};
};

template <>
class TaskPromise<void> : public TaskPromiseBase<TaskPromise<void>> {
 public:
    using task_type = Task<void>;
    constexpr void return_void() const noexcept {}

 private:
};

}  // namespace cent::async
