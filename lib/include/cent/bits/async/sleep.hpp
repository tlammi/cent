#pragma once

#include <cent/bits/async/task.hpp>
#include <cent/time.hpp>

namespace cent::async {

class [[nodiscard]] Sleeper {
    friend Sleeper sleep_until(time::Point);

 public:
    constexpr bool await_ready() const noexcept { return false; }

    template <class T>
    constexpr void await_suspend(std::coroutine_handle<TaskPromise<T>> h) {
        auto* exec = h.promise().executor();
        exec->sleep_current_until(m_tp);
    }

    constexpr void await_resume() const noexcept {}

 private:
    constexpr Sleeper(time::Point tp) noexcept : m_tp(tp) {}
    time::Point m_tp;
};

Sleeper sleep_for(time::Duration dur);
Sleeper sleep_until(time::Point tp);

}  // namespace cent::async
