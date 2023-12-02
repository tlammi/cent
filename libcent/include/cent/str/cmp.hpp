#pragma once

#include <algorithm>
#include <cent/str/util.hpp>
#include <cstddef>

namespace cent {

template <class L, class R>
constexpr auto threeway_compare_case_insenstive(const L& l, const R& r) {
    auto size = std::min(l.size(), r.size());
    for (size_t i = 0; i < size; ++i) {
        auto ll = to_lower(l[i]);
        auto lr = to_lower(r[i]);
        if (ll < lr) return std::strong_ordering::less;
        if (ll > lr) return std::strong_ordering::greater;
    }
    return l.size() <=> r.size();
}
}  // namespace cent
