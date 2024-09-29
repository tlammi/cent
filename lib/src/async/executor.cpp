#include <cent/bits/async/executor.hpp>
#include <cent/util.hpp>
#include <thread>

namespace cent::async {
bool Executor::done() const noexcept {
    return m_active.empty() && m_sleepers.empty();
}

void Executor::resume() {
    CENT_ASSERT(!done());
    if (!m_active.empty()) {
        m_active.front().resume();
        if (m_active.front().done()) {
            m_active.pop_front();
        } else if (m_scheduled_sleep != time::Point()) {
            auto iter = std::find_if(m_sleepers.begin(), m_sleepers.end(),
                                     [&](const auto& pair) {
                                         return pair.first > m_scheduled_sleep;
                                     });
            m_sleepers.emplace(iter, m_scheduled_sleep,
                               std::move(m_active.front()));
            m_active.pop_front();
        } else {
            m_active.splice(m_active.end(), m_active, m_active.begin());
        }
    } else {
        std::this_thread::sleep_until(m_sleepers.front().first);
        m_active.push_back(std::move(m_sleepers.front().second));
        m_sleepers.pop_front();
    }
}
size_t Executor::task_count() const noexcept {
    return m_active.size() + m_sleepers.size();
}

void Executor::sleep_current_until(time::Point tp) noexcept {
    m_scheduled_sleep = tp;
}
}  // namespace cent::async
