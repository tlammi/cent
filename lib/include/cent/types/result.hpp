#pragma once

#include <cent/types/error.hpp>
#include <expected>

namespace cent {


template <class T>
using Result = std::expected<T, Error>;

}  // namespace cent
