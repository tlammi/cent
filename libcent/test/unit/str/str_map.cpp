#include <gtest/gtest.h>

#include <cent/str.hpp>

TEST(Ctor, Default) { cent::StrMap(); }
TEST(Ctor, Empty) { cent::StrMap(""); }
TEST(Ctor, KeyVal) { cent::StrMap("foo=bar"); }
TEST(Ctor, KeyVal2) { cent::StrMap("foo=bar,bar=baz"); }

TEST(Iterate, Default) {
    size_t count = 0;
    for (const auto [k, v] : cent::StrMap()) {
        (void)k;
        (void)v;
        ++count;
    }
    ASSERT_EQ(count, 0);
}

TEST(Iterate, Empty) {
    size_t count = 0;
    for (const auto [k, v] : cent::StrMap("")) {
        (void)k;
        (void)v;
        ++count;
    }
    ASSERT_EQ(count, 0);
}

TEST(Iterate, One) {
    size_t count = 0;
    for (const auto [k, v] : cent::StrMap("foo=bar")) {
        ASSERT_EQ(k, "foo");
        ASSERT_EQ(v, "bar");
        ++count;
    }
    ASSERT_EQ(count, 1);
}

TEST(Iterate, Multiple) {
    auto s = cent::StrMap("foo=bar,bar=baz,baz=asdf");
    size_t counter = 0;
    std::vector<std::pair<std::string_view, std::string_view>> expected{
        {"foo", "bar"}, {"bar", "baz"}, {"baz", "asdf"}};

    for (const auto [k, v] : s) {
        ASSERT_EQ(k, expected.at(counter).first);
        ASSERT_EQ(v, expected.at(counter).second);
        ++counter;
    }
    ASSERT_EQ(counter, 3);
}

TEST(Iterate, EmptyVal) {
    auto s = cent::StrMap("foo=");
    size_t count = 0;
    for (const auto [k, v] : s) {
        ASSERT_EQ(k, "foo");
        ASSERT_EQ(v, "");
        ++count;
    }
    ASSERT_EQ(count, 1);
}

TEST(Iterate, EmptyKey) {
    auto s = cent::StrMap("=bar");
    size_t count = 0;
    for (const auto [k, v] : s) {
        ASSERT_EQ(k, "");
        ASSERT_EQ(v, "bar");
        ++count;
    }
    ASSERT_EQ(count, 1);
}

TEST(Iterate, NoEq) {
    auto s = cent::StrMap("foo");
    size_t count = 0;
    for (const auto [k, v] : s) {
        ASSERT_EQ(k, "foo");
        ASSERT_EQ(v, "");
        ++count;
    }
    ASSERT_EQ(count, 1);
}

