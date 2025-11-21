#include <gtest/gtest.h>

#include <cent/strg/storage.hpp>

using namespace cent::strg;

TEST(Init, InMemory) {
    auto s = in_memory_storage();
    ASSERT_TRUE(s);
}

TEST(Insert, Manifest) {}
