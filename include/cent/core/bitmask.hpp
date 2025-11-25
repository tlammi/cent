#pragma once

#include <cent/core/cast.hpp>
#include <cent/core/concepts.hpp>

namespace cent {

template <enum_type E>
class BitMask {
 public:
    using value_type = std::underlying_type_t<E>;
    constexpr explicit BitMask(E e) noexcept : m_v(underlying_cast(e)) {}

    constexpr BitMask operator|(E e) const noexcept {
        return BitMask(m_v | underlying_cast(e));
    }

    constexpr BitMask operator&(E e) const noexcept {
        return BitMask(m_v & underlying_cast(e));
    }

    constexpr operator bool() const noexcept { return m_v; }

 private:
    constexpr explicit BitMask(value_type v) noexcept : m_v(v) {}
    value_type m_v;
};
namespace detail {
struct BitMaskBuilder {
    template <enum_type E>
    constexpr BitMask<E> operator|(E e) const noexcept {
        return BitMask(e);
    }
};
}  // namespace detail

consteval auto bitmask() { return detail::BitMaskBuilder(); }

}  // namespace cent
