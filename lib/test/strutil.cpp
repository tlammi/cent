/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include "cent/strutil.hpp"

#include <gmock/gmock.h>
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

TEST(SplitLeft, Split) {
    auto [l, r] = cent::split_left("foo:bar", ":");
    ASSERT_EQ(l, "foo");
    ASSERT_EQ(r, "bar");
}

TEST(SplitLeft, Multiple) {
    auto [l, r] = cent::split_left("foo:bar:baz", ":");
    ASSERT_EQ(l, "foo");
    ASSERT_EQ(r, "bar:baz");
}

TEST(SplitLeft, No) {
    auto [l, r] = cent::split_left("asdf", "foobarbaz");
    ASSERT_EQ(l, "asdf");
    ASSERT_EQ(r, "");
}

TEST(SplitLeft, MultipleChars) {
    auto [l, r] = cent::split_left("foobarbaz", "bar");
    ASSERT_EQ(l, "foo");
    ASSERT_EQ(r, "baz");
}

TEST(SplitLeft, Begin) {
    auto [l, r] = cent::split_left("foobarbaz", "foo");
    ASSERT_EQ(l, "");
    ASSERT_EQ(r, "barbaz");
}

TEST(SplitLeft, End) {
    auto [l, r] = cent::split_left("foobarbaz", "baz");
    ASSERT_EQ(l, "foobar");
    ASSERT_EQ(r, "");
}

TEST(SplitLeft, Longer) {
    auto [l, r] = cent::split_left("foobarbaz", "foobarbazz");
    ASSERT_EQ(l, "foobarbaz");
    ASSERT_EQ(r, "");
}

TEST(SplitLeft, Match) {
    auto [l, r] = cent::split_left("foobarbaz", "foobarbaz");
    ASSERT_EQ(l, "");
    ASSERT_EQ(r, "");
}

TEST(Split, Simple) {
    auto res = cent::split("foo:bar:baz", ":");
    ASSERT_EQ(res.size(), 3);

    ASSERT_THAT(res, testing::ElementsAre("foo", "bar", "baz"));
}

TEST(Split, No) {
    auto res = cent::split("foobarbaz", "aaaa");
    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(res[0], "foobarbaz");
}