#pragma once

#include <cent/bits/async/task.hpp>
#include <exception>
#include <optional>

namespace cent::async {

template <class T>
class TaskPromiseBase {
 public:
    constexpr auto get_return_object() {
        return typename T::task_type{
            std::coroutine_handle<T>::from_promise(*static_cast<T*>(this))};
    }
    constexpr std::suspend_always initial_suspend() const noexcept {
        return {};
    }
    constexpr std::suspend_always final_suspend() const noexcept { return {}; }

    void unhandled_exception() const noexcept { std::terminate(); }

 protected:
    constexpr ~TaskPromiseBase() = default;

 private:
};

template <class T>
class TaskPromise : public TaskPromiseBase<TaskPromise<T>> {
 public:
    using task_type = Task<T>;
    template <class V>
    constexpr void return_value(V&& v) noexcept {
        m_val.emplace(std::forward<V>(v));
    }

    T& value() noexcept { return *m_val; }
    const T& value() const noexcept { return *m_val; }

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
