#pragma once

#include <boost/outcome.hpp>
#include <system_error>

namespace cent::core {

template<class T>
using Result = boost::outcome_v2::result<T, std::error_condition>;

}  // namespace cent::core
