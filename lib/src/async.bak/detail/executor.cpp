#include <cent/bits/async/detail/executor.hpp>
#include <cent/bits/util/assert.hpp>

namespace cent::async::detail {

bool Executor::done() const noexcept { return m_enabled.empty(); }

void Executor::resume() {
    m_idx = m_idx % m_enabled.size();
    auto& t = m_enabled.at(m_idx);
    t.resume();
    if (t.done()) {
        if (m_idx < m_enabled.size() - 1)
            std::swap(t, m_enabled.at(m_enabled.size() - 1));
        m_enabled.pop_back();
    } else {
        ++m_idx;
    }
}

auto Executor::disable_current() -> DisabledHandle {}

void Executor::enable(DisabledHandle h) {}

}  // namespace cent::async::detail
