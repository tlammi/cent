#include <gtest/gtest.h>

#include <cent/core.hpp>

TEST(Match, Simple) {
    std::variant<int, char> v{100};
    auto res = cent::match(
        v, [](int i) -> int { return i; }, [](char c) -> int { return -c; });
    ASSERT_EQ(res, 100);
}
