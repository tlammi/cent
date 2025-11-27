#include <gtest/gtest.h>

#include <cent/strg/storage.hpp>

using namespace cent::strg;

TEST(Init, InMemory) {
    auto s = in_memory_storage();
    ASSERT_TRUE(s);
}

TEST(Layers, Init) {
    auto s = in_memory_storage();
    auto l = s->layers();
    (void)l;
}
