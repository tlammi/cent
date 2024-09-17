#include <gtest/gtest.h>

#include <cent/net.hpp>

TEST(Init, Default) {
    auto hdr = cent::net::Headers();
    ASSERT_EQ(hdr.size(), 0);
}

TEST(Init, KeyValue) {
    auto hdr = cent::net::Headers{{"foo", "bar"}, {"bar", "baz"}};
    ASSERT_EQ(hdr.size(), 2);
}

TEST(Init, Values) {
    auto hdr = cent::net::Headers{"foo: bar", "bar: baz", {"baz: asd"}};
    ASSERT_EQ(hdr.size(), 3);
}
