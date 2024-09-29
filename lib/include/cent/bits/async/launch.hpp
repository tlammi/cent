#pragma once

#include <cent/bits/async/task.hpp>

namespace cent::async {

class Launcher {
    friend Launcher launch(Task<void>);

 public:
    Launcher(Task<void> t) : m_t(std::move(t)) {}
    constexpr bool await_ready() const noexcept { return false; }
    template <class T>
    constexpr void await_suspend(
        std::coroutine_handle<TaskPromise<T>> h) noexcept {
        auto* exec = h.promise().executor();
        CENT_ASSERT(exec);
        exec->schedule(std::move(m_t));
    }
    constexpr void await_resume() const noexcept {}

 private:
    Task<void> m_t;
};

Launcher launch(Task<void> t);

}  // namespace cent::async
