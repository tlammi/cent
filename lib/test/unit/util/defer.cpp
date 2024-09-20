#include <gtest/gtest.h>

#include <cent/util.hpp>

TEST(Defer, Call) {
    size_t counter = 0;
    {
        auto d = cent::Defer([&] { ++counter; });
        ASSERT_EQ(counter, 0);
    }
    ASSERT_EQ(counter, 1);
}

TEST(Defer, Cancel) {
    size_t counter = 0;
    {
        auto d = cent::Defer([&] { ++counter; });
        d.cancel();
    }
    ASSERT_EQ(counter, 0);
}
