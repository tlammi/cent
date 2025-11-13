#pragma once

#include <utility>

namespace cent::util {

template <class Fn>
class [[nodiscard]] Defer {
 public:
    constexpr explicit Defer(Fn f) noexcept : m_fn(std::move(f)) {}
    Defer(const Defer&) = delete;
    Defer& operator=(const Defer&) = delete;
    Defer(Defer&&) = delete;
    Defer& operator=(Defer&&) = delete;
    constexpr ~Defer() noexcept(noexcept(m_fn())) {}
    constexpr void cancel(bool c = true) noexcept { m_cancelled = c; }

 private:
    Fn m_fn;
    bool m_cancelled{false};
};
}  // namespace cent::util
