#include "cent/cast.hpp"

#include <gtest/gtest.h>

TEST(Cast, TooLarge) {
    auto f = []() { cent::int_cast<int8_t>(128); };
    ASSERT_ANY_THROW(f());
}

TEST(Cast, TooSmall) {
    auto f = []() { cent::int_cast<uint8_t>(-1); };
    ASSERT_ANY_THROW(f());
}

TEST(Cast, Ok) { cent::int_cast<int8_t>(uint8_t{127}); }