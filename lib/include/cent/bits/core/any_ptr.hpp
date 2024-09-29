#pragma once

#include <cent/bits/core/ptr.hpp>
#include <cent/util.hpp>
#include <variant>

namespace cent {

template <class T>
class AnyPtr {
    using Var = std::variant<T*, UniqPtr<T>, ShrdPtr<T>>;

 public:
    constexpr AnyPtr() noexcept = default;

    constexpr AnyPtr(T* ptr) noexcept : m_var(ptr) {}
    constexpr AnyPtr(UniqPtr<T> ptr) noexcept : m_var(std::move(ptr)) {}
    constexpr AnyPtr(ShrdPtr<T> ptr) noexcept : m_var(std::move(ptr)) {}

    constexpr operator bool() const noexcept { return get(); }

    template <class Self>
    constexpr auto& operator*(this Self&& self) noexcept {
        return *std::forward<Self>(self).get();
    }

    template <class Self>
    constexpr auto* operator->(this Self&& self) noexcept {
        return std::forward<Self>(self).get();
    }

    template <class Self>
    constexpr auto* get(this Self&& self) noexcept {
        return match(std::forward<Self>(self)->m_var, [](auto&& v) {
            if constexpr (std::same_as<std::remove_const_t<decltype(v)>, T*>) {
                return std::forward<decltype(v)>(v);
            } else {
                return std::forward<decltype(v)>(v).get();
            }
        });
    }

 private:
    Var m_var{};
};
}  // namespace cent
