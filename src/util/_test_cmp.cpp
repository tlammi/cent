#include <gtest/gtest.h>

#include "cmp.hpp"

// NOLINTBEGIN

TEST(AnyOf, One) {
    auto res = 0 == cent::any_of(0);
    ASSERT_TRUE(res);
}

TEST(AnyOf, Multiple) {
    const auto cmp = cent::any_of(1, 2, 3, 4, 5);
    ASSERT_NE(0, cmp);
    ASSERT_EQ(1, cmp);
    ASSERT_EQ(2, cmp);
    ASSERT_EQ(3, cmp);
    ASSERT_EQ(4, cmp);
    ASSERT_EQ(5, cmp);
    ASSERT_NE(6, cmp);
}

TEST(NoneOf, One) {
    auto res = 0 == cent::none_of(0);
    ASSERT_FALSE(res);
}

TEST(NoneOf, Multiple) {
    const auto cmp = cent::none_of(1, 2, 3, 4, 5);
    ASSERT_EQ(0, cmp);
    ASSERT_NE(1, cmp);
    ASSERT_NE(2, cmp);
    ASSERT_NE(3, cmp);
    ASSERT_NE(4, cmp);
    ASSERT_NE(5, cmp);
    ASSERT_EQ(6, cmp);
}

TEST(AllOf, One) {
    auto res = 0 == cent::all_of(0);
    ASSERT_TRUE(res);
}

TEST(AllOf, Multiple) {
    auto cmp = cent::all_of(1, 2, 3, 4, 5);
    ASSERT_NE(0, cmp);
    ASSERT_NE(1, cmp);
    ASSERT_NE(2, cmp);
    ASSERT_NE(3, cmp);
    ASSERT_NE(4, cmp);
    ASSERT_NE(5, cmp);
    ASSERT_NE(6, cmp);

    struct Foo {
        bool operator==(int i) const noexcept {
            if (i < 0) return false;
            if (i > 5) return false;
            return true;
        }
    };

    Foo f{};
    ASSERT_EQ(f, cmp);
}

TEST(AnyOf, NoCopyNoMove) {
    struct NoCopyNoMove {
        int i;
        NoCopyNoMove(int i) : i{i} {}

        NoCopyNoMove(const NoCopyNoMove&) = delete;
        NoCopyNoMove& operator=(const NoCopyNoMove&) = delete;
        NoCopyNoMove(NoCopyNoMove&&) = delete;
        NoCopyNoMove& operator=(NoCopyNoMove&&) = delete;

        bool operator==(int other) const noexcept { return i == other; }
    };

    auto a = NoCopyNoMove(1);
    auto b = NoCopyNoMove(2);
    bool res = 1 == cent::any_of(std::cref(a), std::cref(b));
    ASSERT_TRUE(res);
}

// NOLINTEND
