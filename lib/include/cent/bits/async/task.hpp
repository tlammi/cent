#pragma once

#include <coroutine>
#include <exception>
#include <utility>

namespace cent::async {

template <class T>
class TaskPromise;

template <class T = void>
class Task : public std::coroutine_handle<TaskPromise<T>> {
 public:
    using promise_type = TaskPromise<T>;
    using value_type = T;

    constexpr Task() noexcept = default;
    constexpr explicit Task(std::coroutine_handle<TaskPromise<T>> p)
        : std::coroutine_handle<TaskPromise<T>>(std::move(p)) {}

    constexpr ~Task() { std::coroutine_handle<promise_type>::destroy(); }

 private:
};

template <class T>
constexpr T run(Task<T>& t) {
    while (!t.done()) t.resume();
    if constexpr (!std::same_as<T, void>) { return t.promise().value(); }
}

}  // namespace cent::async
