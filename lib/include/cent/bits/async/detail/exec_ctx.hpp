/**
 * \file
 * */
#pragma once

#include <cent/time.hpp>
#include <coroutine>
#include <memory>
#include <stack>
#include <vector>

namespace cent::async {

template <class T>
class Task;
namespace detail {

/**
 * \brief Execution context
 *
 * Shared by different components in async execution.
 * */
class ExecCtx {
 public:
    using Stack = std::stack<std::coroutine_handle<>,
                             std::vector<std::coroutine_handle<>>>;

    constexpr ExecCtx() noexcept = default;
    ExecCtx(const ExecCtx&) = delete;
    ExecCtx& operator=(const ExecCtx&) = delete;

    ExecCtx(ExecCtx&&) = delete;
    ExecCtx& operator=(ExecCtx&&) = delete;

    virtual ~ExecCtx() = default;

    virtual void schedule(Task<void> t) = 0;

    virtual void resume() = 0;

    virtual bool done() = 0;

    virtual Stack& current_stack() noexcept = 0;

    virtual void sleep_current_until(time::Point tp) = 0;
};

std::unique_ptr<ExecCtx> make_exec_ctx();

}  // namespace detail
}  // namespace cent::async
