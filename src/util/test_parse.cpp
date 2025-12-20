CENT_META(
suite: unit
)

#include <gtest/gtest.h>

#include "parse.hpp"

using cent::util::parse_int;

TEST(Int, Empty) { ASSERT_FALSE(parse_int<int>("")); }
TEST(Int, Positive) { ASSERT_EQ(parse_int<int>("123"), 123); }
TEST(Int, Negative) { ASSERT_EQ(parse_int<int>("-123"), -123); }
TEST(Int, UnsignedPos) { ASSERT_EQ(parse_int<unsigned>("123"), 123); };
TEST(Int, UnsignedNeg) { ASSERT_FALSE(parse_int<unsigned>("-123")); }
TEST(Int, Invalid) { ASSERT_FALSE(parse_int<int>("123a")); }
