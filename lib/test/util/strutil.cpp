/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include "cent/util/strutil.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using cent::util::concat_inline;
using cent::util::join;
using cent::util::join_inline;
using cent::util::split;
using cent::util::split_left;

TEST(Join, Empty) {
    std::vector<std::string_view> v{};
    auto res = join(v, "foobar");
    ASSERT_EQ(res, "");
}

TEST(Join, Strings) {
    std::vector<std::string> v{"foo", "bar", "baz"};

    auto res = join(v, "  ");
    ASSERT_EQ(res, "foo  bar  baz");
}

TEST(Join, One) {
    std::vector<std::string> v{"foo"};
    auto res = join(v, "asdfasdf");
    ASSERT_EQ(res, "foo");
}

TEST(SplitLeft, Split) {
    auto [l, r] = split_left("foo:bar", ":");
    ASSERT_EQ(l, "foo");
    ASSERT_EQ(r, "bar");
}

TEST(SplitLeft, Multiple) {
    auto [l, r] = split_left("foo:bar:baz", ":");
    ASSERT_EQ(l, "foo");
    ASSERT_EQ(r, "bar:baz");
}

TEST(SplitLeft, No) {
    auto [l, r] = split_left("asdf", "foobarbaz");
    ASSERT_EQ(l, "asdf");
    ASSERT_EQ(r, "");
}

TEST(SplitLeft, MultipleChars) {
    auto [l, r] = split_left("foobarbaz", "bar");
    ASSERT_EQ(l, "foo");
    ASSERT_EQ(r, "baz");
}

TEST(SplitLeft, Begin) {
    auto [l, r] = split_left("foobarbaz", "foo");
    ASSERT_EQ(l, "");
    ASSERT_EQ(r, "barbaz");
}

TEST(SplitLeft, End) {
    auto [l, r] = split_left("foobarbaz", "baz");
    ASSERT_EQ(l, "foobar");
    ASSERT_EQ(r, "");
}

TEST(SplitLeft, Longer) {
    auto [l, r] = split_left("foobarbaz", "foobarbazz");
    ASSERT_EQ(l, "foobarbaz");
    ASSERT_EQ(r, "");
}

TEST(SplitLeft, Match) {
    auto [l, r] = split_left("foobarbaz", "foobarbaz");
    ASSERT_EQ(l, "");
    ASSERT_EQ(r, "");
}

TEST(Split, Simple) {
    auto res = split("foo:bar:baz", ":");
    ASSERT_EQ(res.size(), 3);

    ASSERT_THAT(res, testing::ElementsAre("foo", "bar", "baz"));
}

TEST(Split, No) {
    auto res = split("foobarbaz", "aaaa");
    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(res[0], "foobarbaz");
}

TEST(JoinInline, Multiple) {
    auto res = join_inline(",", "foo", "bar", "baz");
    ASSERT_EQ(res, "foo,bar,baz");
}

TEST(JoinInline, Single) {
    auto res = join_inline("foobar", "foo");
    ASSERT_EQ(res, "foo");
}

TEST(JoinInline, None) {
    auto res = join_inline("token");
    ASSERT_EQ(res, "");
}

TEST(ConcatInline, Multiple) {
    auto res = concat_inline("foo", "bar", "baz");
    ASSERT_EQ(res, "foobarbaz");
}

TEST(ConcatInline, Single) {
    auto res = concat_inline("foo");
    ASSERT_EQ(res, "foo");
}

TEST(ConcatInline, None) {
    auto res = concat_inline();
    ASSERT_EQ(res, "");
}