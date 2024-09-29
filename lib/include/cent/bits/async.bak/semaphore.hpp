#pragma once

#include <cent/bits/async/executor.hpp>
#include <coroutine>
#include <cstddef>
#include <vector>

namespace cent::async {

template <size_t Max>
class Semaphore {
 public:
    class Awaitable {
     public:
     private:
    };

    constexpr explicit Semaphore(size_t count) : m_count(count) {}

    constexpr bool await_ready() noexcept {
        if (m_count) {
            --m_count;
            return true;
        }
        return false;
    }

    constexpr auto await_suspend(
        std::coroutine_handle<> awaiter) const noexcept {}

    constexpr auto await_resume() const noexcept {}

    void release() noexcept {
        if (m_awaiters.empty()) {
            m_count = std::min(Max, m_count + 1);
        } else {
        }
    }

 private:
    std::vector<std::coroutine_handle<>> m_awaiters{};
    size_t m_count{};
    Executor* m_exec{};
};

using BinarySemahore = Semaphore<1>;

}  // namespace cent::async
