#include <cent/bits/async/detail/exec_ctx.hpp>
#include <cent/bits/async/task.hpp>
#include <cent/time.hpp>
#include <cent/util.hpp>
#include <list>

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
        if (m_active.empty()) return;
        auto& stack = m_active.front().stack;
        stack.top().resume();
        if (stack.top().done()) {
            stack.pop();
            if (stack.empty()) {
                m_active.front().task.promise().rethrow();
                m_active.pop_front();
            }
            return;
        }
        m_active.splice(m_active.end(), m_active, m_active.begin());
    }

    Stack& current_stack() noexcept final { return m_active.front().stack; }

 private:
    struct StackCtx {
        Task<void> task{};
        Stack stack{};
    };
    std::list<StackCtx> m_active{};
    std::list<StackCtx> m_inactive{};

    using SleepPair = std::pair<time::Point, StackCtx>;

    std::list<SleepPair> m_sleepers{};
};

std::unique_ptr<ExecCtx> make_exec_ctx() {
    return std::make_unique<ExecCtxImpl>();
}
}  // namespace cent::async::detail
