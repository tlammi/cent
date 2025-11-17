#pragma once

#include <cent/error.hpp>
#include <variant>

namespace cent {

template <class T>
    requires(!std::same_as<T, Error>)
class Result {
    using Variant = std::variant<T, Error>;

 public:
    constexpr Result(T t) noexcept(std::is_nothrow_constructible_v<Variant, T>)
        : m_var{std::move(t)} {}

    constexpr Result(Error e) noexcept : m_var(std::move(e)) {}

    constexpr bool has_value() const noexcept { return m_var.index() == 0; }
    constexpr explicit operator bool() const noexcept { return has_value(); }

    void try_throw() {
        if (has_value()) return;
        std::move(std::get<1>(m_var)).raise();
    }

    template <class S>
    decltype(auto) operator*(this S&& self) noexcept {
        return std::get<0>(std::forward<S>(self).m_var);
    }

    template <class S>
    decltype(auto) unpack(this S&& self) {
        self.try_throw();
        return *std::forward<S>(self);
    }

 private:
    Variant m_var{};
};

}  // namespace cent
