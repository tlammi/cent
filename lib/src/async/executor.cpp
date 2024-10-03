#include <cent/bits/async/executor.hpp>
#include <cent/util.hpp>
#include <thread>

namespace cent::async {
/*
bool Executor::done() const noexcept {
  return m_active.empty() && m_sleepers.empty();
}

void Executor::resume() {
  CENT_ASSERT(!done());
  if (!m_active.empty()) {
      std::println("step");
      auto& front = m_active.front();
      auto& top = front.stack.top();
      top.resume();
      if (top.done()) {
          front.stack.pop();
          auto f = std::move(m_active.front());
          m_active.pop_front();
      }
      if (top.done()) {
          auto f = std::move(m_active.front());
          m_active.pop_front();
          f.promise().rethrow();
      } else if (m_deactivation_requested) {
          m_inactive.splice(m_inactive.end(), m_active, m_active.begin());
          m_deactivation_requested = false;
      } else if (m_scheduled_sleep != time::Point()) {
          auto iter = std::find_if(m_sleepers.begin(), m_sleepers.end(),
                                   [&](const auto& pair) {
                                       return pair.first > m_scheduled_sleep;
                                   });
          m_sleepers.emplace(iter, m_scheduled_sleep,
                             std::move(m_active.front()));
          m_active.pop_front();
          m_scheduled_sleep = time::Point();
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

auto Executor::deactivate_current() -> DeactivatedHandle {
  m_deactivation_requested = true;
  return m_active.front().handle();
}

void Executor::reactivate(DeactivatedHandle handle) {
  auto iter =
      std::find_if(m_inactive.begin(), m_inactive.end(),
                   [&](const auto& item) { return item.handle() == handle; });
  CENT_ASSERT(iter != m_inactive.end());
  m_active.splice(m_active.end(), m_inactive, iter);
}

*/
}  // namespace cent::async
