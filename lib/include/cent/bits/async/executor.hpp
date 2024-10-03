#pragma once

#include <cent/bits/async/detail/exec_ctx.hpp>
#include <cent/bits/async/task.hpp>
#include <cent/time.hpp>
#include <cent/util.hpp>
#include <list>
#include <stack>
#include <vector>

namespace cent::async {

class Executor {
 public:
    using Stack = std::stack<std::coroutine_handle<>,
                             std::vector<std::coroutine_handle<>>>;

    using DeactivatedHandle = std::coroutine_handle<TaskPromise<void>>;

    constexpr Executor() noexcept = default;

    Executor(const Executor&) = delete;
    Executor& operator=(const Executor&) = delete;

    constexpr Executor(Executor&&) = default;
    constexpr Executor& operator=(Executor&&) = default;

    ~Executor() = default;

    template <std::convertible_to<Task<void>>... Ts>
    Executor(Ts&&... ts) {
        (schedule(std::forward<Ts>(ts)), ...);
    }

    void schedule(Task<void> t) { m_ctx->schedule(std::move(t)); }

    bool done() const noexcept { return m_ctx->done(); }
    void resume() {
        CENT_ASSERT(m_ctx);
        m_ctx->resume();
    }

    // void sleep_current_until(time::Point tp) noexcept;

    // DeactivatedHandle deactivate_current();
    ////void reactivate(DeactivatedHandle handle);

    const detail::ExecCtx* context() const noexcept { return m_ctx.get(); }
    detail::ExecCtx* context() noexcept { return m_ctx.get(); }

 private:
    std::unique_ptr<detail::ExecCtx> m_ctx{detail::make_exec_ctx()};
};
}  // namespace cent::async

