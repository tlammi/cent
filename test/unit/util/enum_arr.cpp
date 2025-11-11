#include <gtest/gtest.h>

#include <cent/util/enum_arr.hpp>

enum class Enum {
    A,
    B,
    C,
};

using cent::util::EnumArr;

TEST(EnumArr, Size) {
    auto a = EnumArr<int, Enum>();
    ASSERT_EQ(a.size(), 3);
}
TEST(EnumArr, Access) {
    auto a = EnumArr<int, Enum>{};
    a[Enum::A] = 1;
    ASSERT_EQ(a[Enum::A], 1);
}

TEST(EnumArr, InitValues) {
    using enum Enum;
    auto a = EnumArr<std::string, Enum>{std::in_place, "A", "B", "C"};
    ASSERT_EQ(a[A], "A");
    ASSERT_EQ(a[B], "B");
    ASSERT_EQ(a[C], "C");
}
