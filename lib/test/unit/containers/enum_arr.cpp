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

