#include <cent/bits/async/detail/exec_ctx.hpp>
#include <cent/bits/async/task.hpp>
#include <cent/time.hpp>
#include <cent/util.hpp>
#include <list>
#include <thread>

namespace cent::async::detail {

class ExecCtxImpl final : public ExecCtx {
 public:
    void schedule(Task<void> t) final {
        t.handle().promise().executor(this);
        auto stack = Stack({t.handle()});
        m_active.emplace_back(std::move(t), std::move(stack));
    }

    bool done() final { return m_active.empty() && m_sleepers.empty(); }
    void resume() final {
        CENT_ASSERT(!done());
        check_sleepers();
        if (m_active.empty()) { wait_next(); }
        auto& stack = m_active.front().stack;
        stack.top().resume();
        if (stack.top().done()) {
            stack.pop();
            if (stack.empty()) {
                m_active.front().task.promise().rethrow();
                m_active.pop_front();
            }
            return;
        } else if (m_deactivation_requested) {
            m_inactive.splice(m_inactive.end(), m_active, m_active.begin());
            m_deactivation_requested = false;
            return;
        } else if (m_sleep_requested != time::Point()) {
            m_sleepers.emplace_back(m_sleep_requested,
                                    std::move(m_active.front()));
            m_active.pop_front();
            m_sleep_requested = time::Point();
            return;
        }
        m_active.splice(m_active.end(), m_active, m_active.begin());
    }

    Stack& current_stack() noexcept final { return m_active.front().stack; }

    void sleep_current_until(time::Point tp) final {}

    DeactivatedHandle* deactivate_current() final {
        m_deactivation_requested = true;
        return static_cast<DeactivatedHandle*>(
            m_active.front().task.handle().address());
    }

    void reactivate(DeactivatedHandle* handle) final {
        void* ptr = static_cast<void*>(handle);
        auto iter = m_inactive.begin();
        while (iter != m_inactive.end()) {
            if (ptr == iter->task.handle().address()) {
                m_active.splice(m_active.end(), m_inactive, iter);
                return;
            }
            ++iter;
        }
        CENT_ASSERT(false);
    }

 private:
    void check_sleepers() {
        auto now = time::Clock::now();
        while (!m_sleepers.empty()) {
            if (now > m_sleepers.front().first) return;
            m_active.emplace_back(std::move(m_sleepers.front().second));
            m_sleepers.pop_front();
        }
    }

    void wait_next() {
        CENT_ASSERT(!m_sleepers.empty());
        std::this_thread::sleep_until(m_sleepers.front().first);
        m_active.emplace_back(std::move(m_sleepers.front().second));
        m_sleepers.pop_front();
    }
    struct StackCtx {
        Task<void> task{};
        Stack stack{};
    };
    std::list<StackCtx> m_active{};
    std::list<StackCtx> m_inactive{};

    using SleepPair = std::pair<time::Point, StackCtx>;

    std::list<SleepPair> m_sleepers{};
    time::Point m_sleep_requested{};
    bool m_deactivation_requested{};
};

std::unique_ptr<ExecCtx> make_exec_ctx() {
    return std::make_unique<ExecCtxImpl>();
}
}  // namespace cent::async::detail
