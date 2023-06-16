#pragma once

#include <type_traits>
#include <utility>
#include <variant>
namespace cent {

template <class V, class... Fns>
decltype(auto) match(V&& v, Fns&&... fns) {
    struct Visitor final : public Fns... {
        Visitor(Fns&&... fns) : Fns{std::forward<Fns>(fns)}... {}
        using Fns::operator()...;
    };

    return std::visit(Visitor{std::forward<Fns>(fns)...}, std::forward<V>(v));
}

template <class E>
constexpr decltype(auto) underlying_cast(E e) {
    static_assert(std::is_enum_v<E>, "needs to be enum");
    return static_cast<std::underlying_type_t<E>>(e);
}

template <class E>
constexpr decltype(auto) underlying_cast(std::underlying_type_t<E> t) {
    static_assert(std::is_enum_v<E>, "needs to be enum");
    return static_cast<E>(t);
}
}  // namespace cent
