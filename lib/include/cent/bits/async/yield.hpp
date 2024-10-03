/**
 * \file
 * */
#pragma once
#include <coroutine>

namespace cent::async {

struct [[nodiscard]] Yielder {
    constexpr bool await_ready() const noexcept { return false; }
    constexpr void await_suspend(
        std::coroutine_handle<> /*unused*/) const noexcept {}
    constexpr void await_resume() const noexcept {}
};

/**
 * \brief Yield the execution to the resumer
 * */
constexpr Yielder yield() noexcept { return {}; }

}  // namespace cent::async
