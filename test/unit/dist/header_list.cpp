#include <gtest/gtest.h>

#include <cent/dist/http/header_list.hpp>

using cent::dist::http::HeaderList;

TEST(Init, Default) {
    auto list = HeaderList();
    ASSERT_TRUE(list.empty());
    ASSERT_EQ(list.size(), 0);
}

TEST(Add, Simple) {
    auto list = HeaderList();
    list.add("foo", "bar");
    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(list.at("foo"), "bar");
}

TEST(Add, Multiple) {
    auto l = HeaderList();
    l.add("foo", "bar");
    l.add("bar", "baz");
    l.add("baz", "asd");
    ASSERT_EQ(l.size(), 3);
}

TEST(Add, Same) {
    auto l = HeaderList();
    l.add("foo", "bar");
    l.add("foo", "baz");
    l.add("foo", "asd");
    ASSERT_EQ(l.size(), 3);
}

TEST(Clear, Empty) {
    auto list = HeaderList();
    list.clear();
    ASSERT_TRUE(list.empty());
}
TEST(Clear, NonEmpty) {
    auto list = HeaderList();
    list.add("foo", "bar");
    list.add("bar", "baz");
    list.clear();
    ASSERT_TRUE(list.empty());
}
