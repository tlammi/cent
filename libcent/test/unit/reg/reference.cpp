#include <gtest/gtest.h>

#include <cent/reg.hpp>

TEST(Ctor, Default) { cent::reg::Reference{}; }
TEST(Ctor, Str) { cent::reg::Reference{"my.registry/sample:foo"}; }

TEST(Tag, Simple) {
    auto r = cent::reg::Reference{"my.registry/sample:foo"};
    ASSERT_EQ(r.tag(), "foo");
}

TEST(Tag, Latest) {
    auto r = cent::reg::Reference{"my.registry/sample"};
    ASSERT_EQ(r.tag(), "latest");
}

TEST(Tag, WithPort) {
    auto r = cent::reg::Reference("localhost:5000/sample:foo");
    ASSERT_EQ(r.tag(), "foo");
}

TEST(Tag, WithPortLatest) {
    auto r = cent::reg::Reference("localhost:5000/sample");
    ASSERT_EQ(r.tag(), "latest");
}

TEST(Tag, Digest) {
    auto r = cent::reg::Reference("my.registry/sample@sha256:deadbeef");
    ASSERT_TRUE(r.tag().empty());
}

TEST(Digest, Tag) {
    auto r = cent::reg::Reference("my.registry/sample:foo");
    ASSERT_TRUE(r.digest().empty());
}

TEST(Digest, Latest) {
    auto r = cent::reg::Reference{"my.registry/sample"};
    ASSERT_TRUE(r.digest().empty());
}

TEST(Digest, Digest) {
    auto r = cent::reg::Reference{"my.registry/sample@sha256:deadbeef"};
    ASSERT_EQ(r.digest(), "sha256:deadbeef");
}

TEST(Digest, WithPort) {
    auto r = cent::reg::Reference("localhost:5000/sample@sha256:deadbeef");
    ASSERT_EQ(r.digest(), "sha256:deadbeef");
}

TEST(Repo, Simple) {
    auto r = cent::reg::Reference{"my.registry/sample:foo"};
    ASSERT_EQ(r.repo(), "my.registry/sample");
}

TEST(Repo, Latest) {
    auto r = cent::reg::Reference{"my.registry/sample"};
    ASSERT_EQ(r.repo(), "my.registry/sample");
}

TEST(Repo, WithPort) {
    auto r = cent::reg::Reference("localhost:5000/sample:foo");
    ASSERT_EQ(r.repo(), "localhost:5000/sample");
}

TEST(Repo, WithPortLatest) {
    auto r = cent::reg::Reference("localhost:5000/sample");
    ASSERT_EQ(r.repo(), "localhost:5000/sample");
}

TEST(Repo, Digest) {
    auto r = cent::reg::Reference{"my.registry/sample@sha256:deadbeef"};
    ASSERT_EQ(r.repo(), "my.registry/sample");
}

