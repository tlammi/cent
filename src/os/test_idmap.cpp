#include <gtest/gtest.h>

#include "detail/idmap.inl"

using namespace cent::os;
using namespace std::literals;

TEST(Split, Simple) {
    auto [user, start, count] = split_subid("foo:1:2"sv);
    ASSERT_EQ(user, "foo");
    ASSERT_EQ(start, "1");
    ASSERT_EQ(count, "2");
}
