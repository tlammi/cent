#pragma once

#include <ranges>

#include "error.hpp"

namespace cent::util {

template <class T, size_t S>
struct ToArray : std::ranges::range_adaptor_closure<ToArray<T, S>> {
    constexpr auto operator()(auto range) const {
        std::array<T, S> out{};
        size_t count = 0;
        auto it = std::ranges::begin(range);
        auto end = std::ranges::end(range);
        while (true) {
            if (it == end || count == out.size()) break;
            out[count] = T(*it);
            ++it;
            ++count;
        }
        if (count != out.size())
            raise(ErrorCode::InvalidArgument, "Missing entry");
        if (it != end) raise(ErrorCode::InvalidArgument, "Extraneous entry");
        return out;
    }
};

template <class T, size_t S>
constexpr auto to_array() {
    return ToArray<T, S>();
}

}  // namespace cent::util
