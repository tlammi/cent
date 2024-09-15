#pragma once

#include <type_traits>
namespace cent::concepts {

template <class E>
concept enum_type = std::is_enum_v<E>;

}
