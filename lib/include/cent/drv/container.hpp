#pragma once

#include <map>
#include <string>
#include <vector>

namespace cent::drv {

template <class T>
using Container = std::map<std::string, T, std::less<>>;

template <class T>
inline Container<T> g_container{};  // NOLINT

template <class T>
std::vector<std::string_view> container_names(const Container<T>& c) {
    std::vector<std::string_view> out{};
    out.reserve(c.size());
    for (const auto& [k, _] : c) { out.push_back(k); }
    return out;
}
}  // namespace cent::drv