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

TEST(BlobWrite, Empty) {
    auto s = in_memory_storage();
    { auto stream = BlobWriteStream(*s, "foo", 0); }
    ASSERT_TRUE(s->has_blob("foo"));
}

TEST(BlobWrite, Simple) {
    auto s = in_memory_storage();
    {
        auto stream = BlobWriteStream(*s, "foo", 10);
        stream.write("foo");
        stream.write("bar");
    }

    auto data = s->read_full_blob("foo");
    ASSERT_EQ(data.size(), 10);
    auto view = std::string_view(reinterpret_cast<const char*>(data.data()), 6);
    ASSERT_EQ(view, "foobar");
}

TEST(BlobRead, Empty) {
    auto s = in_memory_storage();
    s->write_full_blob("foo", "");
    {
        auto stream = BlobReadStream(*s, "foo");
        auto res = stream.read();
        ASSERT_EQ(res.size(), 0);
    }
}

TEST(BlobRead, Simple) {
    auto s = in_memory_storage();
    s->write_full_blob("foo", "foobar");
    {
        auto stream = BlobReadStream(*s, "foo");
        auto res = stream.read();
        ASSERT_EQ(res.size(), 6);
        auto sv = std::string_view(reinterpret_cast<const char*>(res.data()),
                                   res.size());
        ASSERT_EQ(sv, "foobar");
    }
}
