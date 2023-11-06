
#include <gtest/gtest.h>

#include <cent/core.hpp>

using cent::core::match;

TEST(Match, Single) {
    std::variant<int> v{100};
    auto res = match(v, [](int i) { return i * 2; });
    ASSERT_EQ(res, 200);
}

