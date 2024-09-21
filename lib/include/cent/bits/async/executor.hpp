/**
 * \file
 * */
#pragma once

#include <algorithm>
#include <cent/bits/async/task.hpp>
#include <cent/util.hpp>
#include <vector>

namespace cent::async {

/**
 * \brief Single threaded async executor
 * */
class Executor {
 public:
    constexpr Executor() noexcept = default;

    template <class... Ts>
        requires((std::same_as<std::remove_cvref_t<Ts>, Task<void>> && ...))
    constexpr Executor(Ts&&... ts)
        : m_queue(make_vector<Task<void>>(std::forward<Ts>(ts)...)) {}

    constexpr void resume() {
        CENT_ASSERT(!done());
        m_offset = m_offset % m_queue.size();
        auto& t = m_queue.at(m_offset);
        t.resume();
        if (t.done()) erase_done();
    }

    constexpr bool done() const noexcept { return m_queue.empty(); }

    constexpr void run() {
        while (!done()) resume();
    }

 private:
    constexpr void erase_done() noexcept {
        auto iter =
            std::stable_partition(m_queue.begin(), m_queue.end(),
                                  [](const auto& v) { return !v.done(); });
        CENT_ASSERT(m_queue.end() - iter == 1);
        m_queue.erase(iter, m_queue.end());
    }

    std::vector<Task<void>> m_queue{};
    size_t m_offset{0};
};

}  // namespace cent::async
