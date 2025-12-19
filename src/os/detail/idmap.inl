#pragma once

#include <ranges>
#include <string_view>

#include "error.hpp"
#include "util/ranges.hpp"

namespace cent::os {
namespace {

constexpr auto split_subid(auto line) {
    using namespace std::views;
    return line | split(':') | util::to_array<std::string_view, 3>();
}

// Returns range of std::array<std::string_view, 3>
constexpr auto split_subids(std::string_view data) {
    using namespace std::views;
    return data | split('\n') |
           transform([](auto i) { return split_subid(i); });
}

}  // namespace
}  // namespace cent::os
