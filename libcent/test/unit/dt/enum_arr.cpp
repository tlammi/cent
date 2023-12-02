#include <gtest/gtest.h>

#include <cent/dt/enum_arr.hpp>

enum class Enum {
    A,
    B,
    C,
    COUNT_,
};

TEST(EnumArr, Empty) { cent::EnumArr<Enum, int> e{1, 2, 3}; }
