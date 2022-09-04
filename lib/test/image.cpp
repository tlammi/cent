#include "cent/image.hpp"

#include <gtest/gtest.h>

using cent::Image;

TEST(Str, Str) {
    Image i{"docker.io/foo"};
    ASSERT_EQ(i.str(), "docker.io/foo");
    i = "foo";
    ASSERT_EQ(i.str(), "foo");
}

TEST(Tag, Latest) {
    static constexpr const char* IMAGES[] = {
        "docker.io/foo", "foo", "localhost:5000/bar", "bar:latest"};
    for (const auto& i : IMAGES) { ASSERT_EQ(Image(i).tag(), "latest"); }
}

TEST(Tag, Set) {
    struct TestPair {
        const char* input;
        const char* expected;
    };
    static constexpr TestPair TEST_SET[] = {{"docker.io/foo:bar", "bar"},
                                            {"localhost:5000/foo:bar", "bar"}};
    for (const auto& [input, expected] : TEST_SET) {
        ASSERT_EQ(Image(input).tag(), expected);
    }
}

TEST(Registry, Unset) {
    Image i{"foo"};
    ASSERT_EQ(i.registry(), "");
    i = "foo:bar5000";
    ASSERT_EQ(i.registry(), "");
}

TEST(Registry, Set) {
    Image i{"docker.io/foo"};
    ASSERT_EQ(i.registry(), "docker.io");
    i = "localhost:5000/file:400text";
    ASSERT_EQ(i.registry(), "localhost:5000");
}

TEST(Name, A) {
    Image i{"docker.io/foo"};
    ASSERT_EQ(i.name(), "foo");
    i = "localhost/foo/bar/baz:tag";
    ASSERT_EQ(i.name(), "foo/bar/baz");
}

TEST(Repo, A) {
    Image i{"docker.io/foo"};
    ASSERT_EQ(i.repo(), "docker.io/foo");
    i = "docker.io/bar:foo";
    ASSERT_EQ(i.repo(), "docker.io/bar");
}

TEST(Digest, Yes) {
    Image i{"docker.io/foo@sha256:deadbeef"};
    ASSERT_EQ(i.tag(), "");
    ASSERT_EQ(i.digest().str(), "sha256:deadbeef");
}

TEST(Digest, No) {
    Image i{"docker.io/foo:bar"};
    ASSERT_EQ(i.tag(), "bar");
    ASSERT_EQ(i.digest().str(), "");
}