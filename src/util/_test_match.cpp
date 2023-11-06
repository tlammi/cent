
#include <gtest/gtest.h>

#include "match.hpp"

TEST(Match, Single) {
    std::variant<int> v{100};
    auto res = cent::match(v, [](int i) { return i * 2; });
    ASSERT_EQ(res, 200);
}

