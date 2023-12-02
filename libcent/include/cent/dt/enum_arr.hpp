#pragma once

#include <array>
#include <cent/concepts.hpp>
#include <cent/util/cast.hpp>

namespace cent {

template <concepts::enum_type E, class T,
          size_t Size = underlying_cast(E::COUNT_)>
class EnumArr {
 public:
    template <std::convertible_to<T>... Ts>
    EnumArr(Ts&&... ts) : m_arr{std::forward<Ts>(ts)...} {}

 private:
    std::array<T, Size> m_arr;
};

}  // namespace cent
