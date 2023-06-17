#include <gtest/gtest.h>

#include <cent/core/enum_arr.hpp>

#include "cent/core/util.hpp"

enum class Enum : std::uint32_t {
    A,
    B,
    C,
    D,
    COUNT_,
};

TEST(Init, Default) {
    cent::EnumArr<Enum, int> a{};
    ASSERT_EQ(a.size(), cent::underlying_cast(Enum::COUNT_));
}

TEST(Init, Values) {
    cent::EnumArr<Enum, int> a{1, 2, 3, 4};
    ASSERT_EQ(a.size(), cent::underlying_cast(Enum::COUNT_));
}

TEST(Init, MakeArray) {
    using enum Enum;
    auto arr = cent::make_enum_arr<Enum, int>(A, 1, B, 2, C, 3, D, 4);
    ASSERT_EQ(arr[A], 1);
    ASSERT_EQ(arr[B], 2);
}

TEST(EnumArr, Access) {
    using enum Enum;
    cent::EnumArr<Enum, int> a{1, 2, 3, 4};
    ASSERT_EQ(a[A], 1);
    ASSERT_EQ(a[B], 2);
    a[A] = 10;
    ASSERT_EQ(a[A], 10);
    ASSERT_EQ(a[B], 2);
    ASSERT_EQ(a[C], 3);
}

