#include <gtest/gtest.h>

#include <cent/name.hpp>

using cent::Name;

TEST(Ctor, Default) {
    auto nm = Name();
    ASSERT_TRUE(nm.registry().empty());
    ASSERT_TRUE(nm.repository().empty());
    ASSERT_TRUE(nm.digest().empty());
}

TEST(Ctor, Full) {
    auto nm = Name("docker.io/foo/bar:baz");
    ASSERT_EQ(nm.registry(), "docker.io");
    ASSERT_EQ(nm.repository(), "foo/bar");
    ASSERT_EQ(nm.tag(), "baz");
    ASSERT_EQ(nm.suffix(), "baz");
    ASSERT_EQ(nm.digest(), "");
    ASSERT_TRUE(nm.has_tag());
}
