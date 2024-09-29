/**
 * \file
 * */
#pragma once

#include <coroutine>
#include <vector>

namespace cent::async {

/**
 * \brief Type to wait for an event
 *
 * Events can be used for waiting on external resources. For instance an
 * external library callback can set an event which causes then the awaiting
 * coroutines to resume. Multiple coroutines can wait for a single event.
 * */
class Event {
 public:
    constexpr Event() noexcept = default;

    constexpr void set() noexcept { m_set = true; }

    constexpr bool await_ready() const noexcept { return m_set; }

    auto await_suspend(std::coroutine_handle<> awaiting_coroutine) noexcept
        -> std::coroutine_handle<> {
        m_awaiters.emplace_back(std::move(awaiting_coroutine));
        m_h.promise().continuation(awaiting_coroutine);
        return m_h;
    }

 private:
    std::vector<std::coroutine_handle<>> m_awaiters{};
    bool m_set{false};
};

}  // namespace cent::async
