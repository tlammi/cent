#include <cent/bits/async/executor.hpp>
#include <cent/util.hpp>

namespace cent::async {
bool Executor::done() const noexcept { return m_active.empty(); }

void Executor::resume() {
    CENT_ASSERT(!done());
    m_active.front().resume();
    if (m_active.front().done()) {
        m_active.pop_front();
    } else {
        m_active.splice(m_active.end(), m_active, m_active.begin());
    }
}

size_t Executor::task_count() const noexcept { return m_active.size(); }

}  // namespace cent::async
