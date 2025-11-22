#include <gtest/gtest.h>

#include <cent/strg/storage.hpp>

using namespace cent::strg;

TEST(Init, InMemory) {
    auto s = in_memory_storage();
    ASSERT_TRUE(s);
}

TEST(Insert, Manifest) {
    auto s = in_memory_storage();
    s->set_manifest("foo", "bar");
    ASSERT_TRUE(s->has_manifest("foo"));
    ASSERT_EQ(s->manifest("foo"), "bar");
}

TEST(Insert, ExistingManifest) {
    auto s = in_memory_storage();
    s->set_manifest("foo", "bar");
    s->set_manifest("foo", "baz");
    ASSERT_TRUE(s->has_manifest("foo"));
    ASSERT_EQ(s->manifest("foo"), "baz");
}

TEST(Transaction, Cancel) {
    auto s = in_memory_storage();
    {
        auto t = Transaction(*s);
        s->set_manifest("foo", "bar");
    }
    ASSERT_FALSE(s->has_manifest("foo"));
}

TEST(Transaction, Commit) {
    auto s = in_memory_storage();
    {
        auto t = Transaction(*s);
        s->set_manifest("foo", "bar");
        t.commit();
    }
    ASSERT_EQ(s->manifest("foo"), "bar");
}
