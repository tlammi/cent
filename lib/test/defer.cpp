/* SPDX-License-Identifier:  GPL-3.0-or-later */
#include "cent/defer.hpp"

#include <gtest/gtest.h>

#include <functional>

TEST(Defer, Empty) {
    cent::Defer d{};
    (void)d;
}

TEST(Defer, Nop) {
    cent::Defer d{[]() {}};
    (void)d;
}

TEST(Defer, Lambda) {
    int i = 0;
    {
        cent::Defer d{[&]() { i = 100; }};
        ASSERT_EQ(i, 0);
    }
    ASSERT_EQ(i, 100);
}

TEST(Defer, Function) {
    int i = 0;
    {
        cent::Defer d{std::function{[&]() { i = 100; }}};
        ASSERT_EQ(i, 0);
    }
    ASSERT_EQ(i, 100);
}

TEST(Defer, Ref) {
    int i = 0;
    auto noncopyable = [&, ptr = std::make_unique<int>(100)]() { i = *ptr; };
    {
        cent::Defer d{noncopyable};
        ASSERT_EQ(i, 0);
    }
    ASSERT_EQ(i, 100);
}

TEST(Defer, Move) {
    int i = 0;
    auto noncopyable = [&, ptr = std::make_unique<int>(100)]() { i = *ptr; };
    {
        cent::Defer d{std::move(noncopyable)};
        ASSERT_EQ(i, 0);
    }
    ASSERT_EQ(i, 100);
}