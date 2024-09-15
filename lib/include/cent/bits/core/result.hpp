#pragma once

#include <cent/bits/core/error.hpp>
#include <expected>

namespace cent {

template <class T>
using Result = std::expected<T, Error>;

}
