#pragma once

#include <type_traits>

namespace cent {

template <class T>
concept enum_type = std::is_enum_v<T>;

}
