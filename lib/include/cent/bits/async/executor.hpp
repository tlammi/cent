#pragma once

#include <cent/bits/async/task.hpp>
#include <cent/util.hpp>
#include <list>

namespace cent::async {

class Executor {
 public:
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

 private:
    std::list<Task<void>> m_active{};
    std::list<Task<void>> m_inactive{};
};

}  // namespace cent::async
