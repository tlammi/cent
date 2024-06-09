#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace cent {
namespace inline_detail {
constexpr auto max_alignment = alignof(std::max_align_t);
}

template <class B, size_t S = inline_detail::max_alignment>
class Inline {
 public:
    constexpr Inline() noexcept = default;

    template <std::derived_from<B> T, class... Ts>
    Inline(std::in_place_type_t<T> /*unused*/, Ts&&... ts) : m_value(true) {
        static_assert(sizeof(T) <= S);
        std::construct_at(buf<T>(), std::forward<Ts>(ts)...);
    }

    template <std::derived_from<B> T>
    explicit Inline(T&& t) : m_value(true) {
        static_assert(sizeof(T) <= S);
        std::construct_at(buf<T>(), std::forward<T>(t));
    }

    Inline(const Inline&) = delete;
    Inline(Inline&&) = delete;

    Inline& operator=(const Inline&) = delete;
    Inline& operator=(Inline&&) = delete;

    constexpr ~Inline() {
        if (m_value) buf<B>()->~B();
    }

    B* operator->() noexcept { return buf<B>(); }
    const B* operator->() const noexcept { return buf<const B>(); }

    B& operator*() noexcept { return *buf<B>(); }
    const B& operator*() const noexcept { return *buf<const B>(); }

 private:
    template <class T>
    T* buf() noexcept {
        return reinterpret_cast<T*>(m_buf.data());
    }
    alignas(alignof(std::max_align_t)) std::array<uint8_t, S> m_buf{};
    bool m_value{false};
};

}  // namespace cent
