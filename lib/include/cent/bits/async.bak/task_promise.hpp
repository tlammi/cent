#pragma once

#include <cent/bits/async/task.hpp>
#include <exception>
#include <optional>
#include <print>
#include <stacktrace>

namespace cent::async {

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

 protected:
    constexpr ~TaskPromiseBase() = default;

 private:
    std::coroutine_handle<> m_cont{};
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
    auto& value(this Self&& self) noexcept {
        return std::forward<Self>(self).value();
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
