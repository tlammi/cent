/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include "cent/util/enum_arr.hpp"

#include <gtest/gtest.h>

#include <string_view>

using cent::util::EnumArr;

enum E {
    A,
    B,
    COUNT_,
};

TEST(EnumArr, Size) {
    const EnumArr<E, const char*> e{"A", "B"};
    ASSERT_EQ(e.size(), 2);
}

TEST(EnumArr, Index) {
    const EnumArr<E, std::string_view> e{"A", "B"};
    using enum E;
    ASSERT_EQ(e[A], "A");
    ASSERT_EQ(e[B], "B");
}