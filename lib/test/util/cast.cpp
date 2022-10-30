/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include "cent/util/cast.hpp"

#include <gtest/gtest.h>

using cent::util::int_cast;
using cent::util::underlying_cast;

TEST(Cast, TooLarge) {
    auto f = []() { int_cast<int8_t>(128); };
    ASSERT_ANY_THROW(f());
}

TEST(Cast, TooSmall) {
    auto f = []() { int_cast<uint8_t>(-1); };
    ASSERT_ANY_THROW(f());
}

TEST(Cast, Ok) { int_cast<int8_t>(uint8_t{127}); }

enum class SizeT : size_t {
    A,
    B,
};

enum class Int : int {
    A,
    B,
};

TEST(Underlying, FromEnum) {
    auto s = underlying_cast(SizeT::B);
    static_assert(std::is_same_v<decltype(s), size_t>);
    ASSERT_EQ(s, 1);
    auto i = underlying_cast(Int::B);
    static_assert(std::is_same_v<decltype(i), int>);
    ASSERT_EQ(i, 1);
}

TEST(Underlying, FromUnderlying) {
    ASSERT_EQ(underlying_cast<SizeT>(1), SizeT::B);
    ASSERT_EQ(underlying_cast<Int>(1), Int::B);
}