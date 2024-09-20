#pragma once
#include <utility>

namespace cent {

template <class T>
class [[nodiscard]] Defer {
 public:
    constexpr explicit Defer(T t) noexcept : m_t(std::move(t)) {}
    Defer(const Defer&) = delete;
    Defer& operator=(const Defer&) = delete;

    Defer(Defer&&) = delete;
    Defer& operator=(Defer&&) = delete;

    constexpr ~Defer() {
        if (!m_cancelled) m_t();
    }

    constexpr void cancel(bool val = true) noexcept { m_cancelled = val; }

 private:
    T m_t;
    bool m_cancelled{false};
};

}  // namespace cent
