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

TEST(Ctor, WithoutTag) {
    auto nm = Name("docker.io/foo/bar");
    ASSERT_EQ(nm.tag(), "latest");
}

TEST(Ctor, Digest) {
    auto nm = Name(
        "docker.io/foo/"
        "bar@sha256:"
        "01ba4719c80b6fe911b091a7c05124b64eeece964e09c058ef8f9805daca546b");

    ASSERT_EQ(
        nm.digest(),
        "sha256:"
        "01ba4719c80b6fe911b091a7c05124b64eeece964e09c058ef8f9805daca546b");
}
