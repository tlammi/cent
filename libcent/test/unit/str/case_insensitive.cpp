#include <gtest/gtest.h>

#include <cent/str.hpp>

using CIs = cent::CaseInsensitive;

TEST(Equal, Empty) {
    CIs c{""};
    ASSERT_EQ(c, "");
}

TEST(Equal, Equal) {
    CIs c{"foo"};
    ASSERT_EQ(c, "foo");
}

TEST(Equal, UpperToLower) {
    CIs c{"FOO"};
    ASSERT_EQ(c, "foo");
}

