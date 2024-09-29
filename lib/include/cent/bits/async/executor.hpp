#pragma once

#include <cent/bits/async/task.hpp>
#include <cent/time.hpp>
#include <cent/util.hpp>
#include <list>

namespace cent::async {

class Executor {
 public:
    using DeactivatedHandle = std::coroutine_handle<TaskPromise<void>>;

    constexpr Executor() noexcept = default;

    Executor(const Executor&) = delete;
    Executor& operator=(const Executor&) = delete;

    Executor(Executor&&) = delete;
    Executor& operator=(Executor&&) = delete;

    ~Executor() = default;

    template <std::convertible_to<Task<void>>... Ts>
    Executor(Ts&&... ts) {
        (schedule(std::forward<Ts>(ts)), ...);
    }

    void schedule(Task<void> t) {
        m_active.push_back(std::move(t));
        m_active.back().promise().executor(this);
    }

    bool done() const noexcept;
    void resume();

    size_t task_count() const noexcept;

    void sleep_current_until(time::Point tp) noexcept;

    DeactivatedHandle deactivate_current();
    void reactivate(DeactivatedHandle handle);

 private:
    std::list<Task<void>> m_active{};
    std::list<Task<void>> m_inactive{};
    std::list<std::pair<time::Point, Task<void>>> m_sleepers{};
    time::Point m_scheduled_sleep{};
    bool m_deactivation_requested{false};
};

}  // namespace cent::async
