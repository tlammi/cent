#include "cent/strutil.hpp"

#include <gtest/gtest.h>

TEST(Join, Empty) {
    std::vector<std::string_view> v{};
    auto res = cent::join(v, "foobar");
    ASSERT_EQ(res, "");
}

TEST(Join, Strings) {
    std::vector<std::string> v{"foo", "bar", "baz"};

    auto res = cent::join(v, "  ");
    ASSERT_EQ(res, "foo  bar  baz");
}

TEST(Join, One) {
    std::vector<std::string> v{"foo"};
    auto res = cent::join(v, "asdfasdf");
    ASSERT_EQ(res, "foo");
}