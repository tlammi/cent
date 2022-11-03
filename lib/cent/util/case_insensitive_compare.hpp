#pragma once

#include <algorithm>
#include <string>
#include <string_view>

namespace cent::util {

/**
 * Case-insensitive string operator for e.g. std::map
 */
class CaseInsensitiveCompare {
 public:
    using is_transparent = void;

    template <class StrA, class StrB>
    auto operator()(StrA&& l, StrB&& r) const
        -> decltype(std::forward<StrA>(l) < std::forward<StrB>(r)) {
        const auto sl = tolower_str(std::forward<StrA>(l));
        const auto rl = tolower_str(std::forward<StrB>(r));
        return sl < rl;
    }

 private:
    template <class StrLike>
    static std::string tolower_str(StrLike&& s) {
        std::string str{};
        std::transform(s.begin(), s.end(), std::back_inserter(str),
                       [](char c) { return std::tolower(c); });
        return str;
    }
};
}  // namespace cent::util