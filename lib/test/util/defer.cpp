/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include "cent/util/defer.hpp"

#include <gtest/gtest.h>

#include <functional>

using cent::util::Defer;

TEST(Defer, Empty) {
    Defer d{};
    (void)d;
}

TEST(Defer, Nop) {
    Defer d{[]() {}};
    (void)d;
}

TEST(Defer, Lambda) {
    int i = 0;
    {
        Defer d{[&]() { i = 100; }};
        ASSERT_EQ(i, 0);
    }
    ASSERT_EQ(i, 100);
}

TEST(Defer, Function) {
    int i = 0;
    {
        Defer d{std::function{[&]() { i = 100; }}};
        ASSERT_EQ(i, 0);
    }
    ASSERT_EQ(i, 100);
}

TEST(Defer, Ref) {
    int i = 0;
    auto noncopyable = [&, ptr = std::make_unique<int>(100)]() { i = *ptr; };
    {
        Defer d{noncopyable};
        ASSERT_EQ(i, 0);
    }
    ASSERT_EQ(i, 100);
}

TEST(Defer, Move) {
    int i = 0;
    auto noncopyable = [&, ptr = std::make_unique<int>(100)]() { i = *ptr; };
    {
        Defer d{std::move(noncopyable)};
        ASSERT_EQ(i, 0);
    }
    ASSERT_EQ(i, 100);
}