#pragma once

#include <algorithm>
#include <cent/str/util.hpp>
#include <cstddef>
#include <string_view>

namespace cent {

template <class L, class R>
constexpr auto threeway_compare_case_insenstive(const L& l, const R& r) {
    std::string_view lv = l;
    std::string_view rv = r;
    while (!lv.empty() && !rv.empty()) {
        auto cl = to_lower(lv.front());
        auto cr = to_lower(rv.front());
        if (cl < cr) return std::strong_ordering::less;
        if (cl > cr) return std::strong_ordering::greater;
        lv.remove_prefix(1);
        rv.remove_prefix(1);
    }
    return lv.size() <=> rv.size();
}
}  // namespace cent
