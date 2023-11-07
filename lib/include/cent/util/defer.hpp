#pragma once

#include <concepts>
#include <utility>

namespace cent {

template <std::invocable F>
class Defer {
 public:
    static_assert(std::is_nothrow_invocable_v<std::remove_cvref_t<F>>,
                  "cannot throw");

    constexpr explicit Defer(F f) : m_f(std::move(f)) {}

    Defer(const Defer&) = delete;
    Defer& operator=(const Defer&) = delete;
    Defer(Defer&&) = delete;
    Defer& operator=(Defer&&) = delete;

    constexpr ~Defer() {
        if (!m_cancelled) m_f();
    }

    constexpr void cancel(bool b = true) noexcept { m_cancelled = b; }

 private:
    F m_f;
    bool m_cancelled = false;
};
}  // namespace cent
