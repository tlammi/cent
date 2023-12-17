#pragma once

#include <cent/dt.hpp>

namespace cent::net {
// TODO: Should be case-insensitive
using Headers = FlatMultimap<std::string, std::string>;
}  // namespace cent::net
