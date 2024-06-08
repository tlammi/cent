#pragma once

#include <cent/types/error.hpp>
#include <expected>

namespace cent {


template <class T>
using Result = std::expected<T, Error>;

template<class... Ts>
constexpr std::unexpected<Error> error(Ts&&... ts){
  return std::unexpected<Error>(std::in_place, std::forward<Ts>(ts)...);
}

}  // namespace cent
