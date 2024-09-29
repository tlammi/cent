#pragma once

#include <cent/bits/async/detail/handle.hpp>
#include <cent/bits/async/task_promise.hpp>
#include <list>
#include <vector>

namespace cent::async::detail {

class Executor {
    using CoroHandle = Handle<TaskPromise<void>>;
    using Vect = std::vector<CoroHandle>;
    using List = std::list<CoroHandle>;

 public:
    using DisabledHandle = List::iterator;

    constexpr Executor() noexcept = default;

    Executor(const Executor&) = delete;
    Executor& operator=(const Executor&) = delete;

    Executor(Executor&&) = delete;
    Executor& operator=(Executor&&) = delete;

    ~Executor() = default;

    bool done() const noexcept;
    void resume();

    DisabledHandle disable_current();

    void enable(DisabledHandle h);

 private:
    Vect m_enabled{};
    List m_disabled{};
    size_t m_idx{0};
};
}  // namespace cent::async::detail
