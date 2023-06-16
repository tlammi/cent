#pragma once

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
}  // namespace cent
