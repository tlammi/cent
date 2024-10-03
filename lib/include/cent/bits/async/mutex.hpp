#pragma once

#include <cent/bits/async/executor.hpp>
#include <cent/bits/async/task.hpp>
#include <list>

namespace cent::async {

class [[nodiscard]] Mutex {
 public:
    class Lock {
        friend class Mutex;

     public:
        Lock(const Lock&) = delete;
        Lock& operator=(const Lock&) = delete;

        constexpr Lock(Lock&& other) noexcept
            : m_mut(std::exchange(other.m_mut, nullptr)) {}

        constexpr Lock& operator=(Lock&& other) noexcept {
            auto tmp = Lock(std::move(other));
            std::swap(m_mut, other.m_mut);
            return *this;
        }

        constexpr ~Lock() {
            if (m_mut) m_mut->unlock();
        }

     private:
        constexpr Lock(Mutex* mut) noexcept : m_mut(mut) {}

        Mutex* m_mut{};
    };
    constexpr Mutex() noexcept = default;

    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;

    Mutex(Mutex&&) = delete;
    Mutex& operator=(Mutex&&) = delete;

    ~Mutex() = default;

    constexpr bool await_ready() const noexcept { return !m_locked; }

    template <class T>
    void await_suspend(std::coroutine_handle<TaskPromise<T>> h) {
        if (!m_locked) {
            m_locked = true;
            return;
        }
        auto* exec = h.promise().executor();
        m_queue.emplace_back(exec, exec->deactivate_current());
    }

    constexpr auto await_resume() noexcept { return Lock{this}; }

 private:
    constexpr void unlock() noexcept {
        if (m_queue.empty()) {
            m_locked = false;
            return;
        }
        auto& first = m_queue.front();
        first.first->reactivate(first.second);
        m_queue.pop_front();
    }

    using Pair =
        std::pair<detail::ExecCtx*, detail::ExecCtx::DeactivatedHandle*>;
    std::list<Pair> m_queue{};
    bool m_locked{false};
};

}  // namespace cent::async
