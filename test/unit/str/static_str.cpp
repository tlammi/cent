#include <gtest/gtest.h>

#include <cent/str.hpp>

TEST(StaticStrArr, DefaultCtor) {
    cent::str::StaticStrArr arr;
    auto v = arr.view();
    ASSERT_EQ(v.size(), 0);
}

TEST(StaticStrArr, CtorOne) {
    cent::str::StaticStrArr arr{"hello"};
    ASSERT_EQ(arr.size(), 6);
    ASSERT_EQ(arr, "hello");
}

TEST(StaticStrArr, CtorTwo) {
    static constexpr cent::str::StaticStrArr arr{"hello", "world"};
    ASSERT_EQ(arr.size(), 11);
    ASSERT_EQ(arr, "helloworld");
}

TEST(StaticStrArr, CtorThree) {
    static constexpr cent::str::StaticStrArr arr{"hello", "world", "!"};
    ASSERT_EQ(arr.size(), 12);
    ASSERT_EQ(arr, "helloworld!");
}

