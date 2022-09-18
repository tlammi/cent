/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include <gtest/gtest.h>

#include "cent/http/header.hpp"

using cent::http::Header;
using cent::http::HeaderView;

TEST(Ctor, A) { Header h{"foo: bar"}; }
TEST(Ctor, B) { Header h{"foo: bar=\"baz\",asdf=\"ghjk\""}; }

TEST(FieldName, A) {
    Header h{"foo: bar"};
    ASSERT_EQ(h.field_name(), "foo");
}

TEST(FieldName, B) {
    Header h{"foo-bar: asdf=ghkl, a"};
    ASSERT_EQ(h.field_name(), "foo-bar");
}

TEST(Value, A) {
    HeaderView h{"foo: bar"};
    ASSERT_EQ(h.value(), "bar");
}

TEST(Value, B) {
    HeaderView h{"foo:        \t\t\t\t asdf asd asdf     "};
    ASSERT_EQ(h.value(), "asdf asd asdf");
}

TEST(SubField, One) {
    HeaderView h{"foo: a=\"b\""};
    ASSERT_EQ(h.sub_value("a"), "\"b\"");
}
TEST(SubField, Multiple) {
    HeaderView h{
        R"(www-authenticate: Bearer realm="https://auth.docker.io/token",service="registry.docker.io",scope="repository:library/ubuntu:pull")"};
    ASSERT_EQ(h.sub_value("Bearer realm"), R"("https://auth.docker.io/token")");
    ASSERT_EQ(h.sub_value("service"), R"("registry.docker.io")");
    ASSERT_EQ(h.sub_value("scope"), R"("repository:library/ubuntu:pull")");
}

TEST(SubField, WhiteSpace) {
    HeaderView h{"foo: bar=baz  "};
    ASSERT_EQ(h.sub_value("bar"), "baz");
}

TEST(HeaderEquals, Lower) {
    Header h{"foo: bar"};
    ASSERT_TRUE(h.header_equals("foo"));
}

TEST(HeaderEquals, Upper) {
    Header h{"FOO: bar"};
    ASSERT_TRUE(h.header_equals("foo"));
}

TEST(HeaderEquals, Mixed) {
    Header h{"fOo: bar"};
    ASSERT_TRUE(h.header_equals("foo"));
}