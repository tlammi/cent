#include "cent/bits/containers/enum_arr.hpp"

#include <gtest/gtest.h>

#include <cent/containers.hpp>

enum class Empty {};
enum class Sequence {
    A,
    B,
    C,
    D,
};

enum class Gaps {
    A = 1,
    B = 3,
    C = 7,
    D = 10,
};

enum class Mixed {
    A = 3,
    B = -1,
    C = 4,
    D = 2,
};

TEST(Ctor, Empty) {
    cent::EnumArr<Empty, int> a{};
    ASSERT_EQ(a.size(), 0);
}

TEST(Ctor, Sequential) {
    cent::EnumArr<Sequence, int> a{};
    ASSERT_EQ(a.size(), 4);
}

TEST(Ctor, Gaps) {
    cent::EnumArr<Sequence, int> a{};
    ASSERT_EQ(a.size(), 4);
}

TEST(Index, Sequence) {
    cent::EnumArr<Sequence, int> a{std::in_place_type<int>, 1, 2, 3, 4};
    ASSERT_EQ(a[Sequence::A], 1);
    ASSERT_EQ(a[Sequence::B], 2);
}

TEST(Index, Gaps) {
    cent::EnumArr<Gaps, int> a{std::in_place_type<int>, 1, 2, 3, 4};
    ASSERT_EQ(a[Gaps::A], 1);
    ASSERT_EQ(a[Gaps::B], 2);
    ASSERT_EQ(a[Gaps::C], 3);
    ASSERT_EQ(a[Gaps::D], 4);
}

TEST(Index, Mixed) {
    cent::EnumArr<Mixed, int> a{std::in_place_type<int>, 1, 2, 3, 4};
    ASSERT_EQ(a[Mixed::B], 1);
    ASSERT_EQ(a[Mixed::D], 2);
    ASSERT_EQ(a[Mixed::A], 3);
    ASSERT_EQ(a[Mixed::C], 4);
}

TEST(MakeArr, Empty) {
    auto arr = cent::make_enum_arr<Mixed, int>();
    static_assert(
        std::same_as<std::remove_cvref_t<decltype(arr)>::enum_type, Mixed>);
}

