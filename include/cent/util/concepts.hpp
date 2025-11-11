#pragma once

#include <type_traits>

namespace cent::util {

template <class T>
concept enum_type = std::is_enum_v<T>;

}
