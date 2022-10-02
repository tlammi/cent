#pragma once

#include <map>
#include <string>

namespace cent::drv {

template <class T>
inline std::map<std::string, T, std::less<>> g_container{};  // NOLINT

}  // namespace cent::drv