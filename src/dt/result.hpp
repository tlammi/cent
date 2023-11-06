#pragma once

#include <expected>

#include "dt/error.hpp"

namespace cent {

template <class T>
using Result = std::expected<T, Error>;
}
