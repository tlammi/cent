#include <gtest/gtest.h>

#include <cent/cent.hpp>

TEST(Setup, Defaults) {
    cent::init_defaults();
    auto c = cent::Cent();
}

TEST(System, Reset) {
    cent::init_defaults();
    auto c = cent::Cent();
    c.system_reset();
}
