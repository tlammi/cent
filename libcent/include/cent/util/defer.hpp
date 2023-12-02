#pragma once

#include <cent/concepts.hpp>
#include <utility>

namespace cent {

template <concepts::proto<void()> F>
class [[nodiscard]] Defer {
 public:
    constexpr Defer(F&& f) : m_f{std::forward<F>(f)} {}

    Defer(const Defer&) = delete;
    Defer& operator=(const Defer&) = delete;

    Defer(Defer&&) = delete;
    Defer& operator=(Defer&&) = delete;

    constexpr ~Defer() {
        if (!m_cancelled) m_f();
    }

    constexpr void cancel(bool cancelled = true) noexcept {
        m_cancelled = cancelled;
    }

 private:
    F m_f;
    bool m_cancelled = false;
};

}  // namespace cent
