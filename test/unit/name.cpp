#include <gtest/gtest.h>

#include <cent/name.hpp>

using cent::Name;

using namespace cent::literals;

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

TEST(ReplaceSuffix, TagToTag) {
    auto nm = Name("docker.io/foo/bar:baz");
    nm.set_tag("asd");
    ASSERT_EQ(nm, "docker.io/foo/bar:asd"_nm)
        << nm.registry() << '|' << nm.repository() << '|' << nm.suffix();
}

TEST(ReplaceSuffix, NoneToTag) {
    auto nm = Name("docker.io/foo/bar");
    nm.set_tag("baz");
    ASSERT_EQ(nm, "docker.io/foo/bar:baz"_nm)
        << nm.registry() << '|' << nm.repository() << '|' << nm.suffix();
}

TEST(ReplaceSuffix, TagToNone) {
    auto nm = Name("docker.io/foo/bar:baz");
    nm.set_tag("");
    ASSERT_EQ(nm, "docker.io/foo/bar:latest"_nm)
        << nm.registry() << '|' << nm.repository() << '|' << nm.suffix();
}

TEST(ReplaceSuffix, DigestToDigest) {
    auto nm = Name("docker.io/foo/bar@baz:asdf");
    nm.set_digest("foo:bar");
    ASSERT_EQ(nm, "docker.io/foo/bar@foo:bar"_nm);
}

TEST(ReplaceSuffix, NoneToDigest) {
    auto nm = Name("docker.io/foo/bar");
    nm.set_digest("foo:bar");
    ASSERT_EQ(nm, "docker.io/foo/bar@foo:bar"_nm);
}

TEST(ReplaceSuffix, DigestToNone) {
    auto nm = Name("docker.io/foo/bar@foo:bar");
    nm.set_digest("");
    ASSERT_EQ(nm, "docker.io/foo/bar:latest"_nm);
}

TEST(ReplaceSuffix, TagToDigest) {
    auto nm = Name("docker.io/foo/bar:baz");
    nm.set_digest("asd:ghj");
    ASSERT_EQ(nm, "docker.io/foo/bar@asd:ghj"_nm);
}

TEST(ReplaceSuffix, DigestToTag) {
    auto nm = Name("docker.io/foo/bar@baz:asd");
    nm.set_tag("tag");
    ASSERT_EQ(nm, "docker.io/foo/bar:tag"_nm);
}
