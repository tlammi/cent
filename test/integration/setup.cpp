#include <gtest/gtest.h>

#include <cent/cent.hpp>

TEST(Setup, Defaults) {
    cent::init_defaults();
    auto c = cent::Cent();
}