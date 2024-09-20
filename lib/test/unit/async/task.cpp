#include <gtest/gtest.h>

#include <cent/async.hpp>

namespace ca = cent::async;

TEST(Init, Void) {
    auto t = []() -> ca::Task<> { co_return; }();
    (void)t;
}

TEST(Init, Int) {
    auto t = []() -> ca::Task<int> { co_return 1; }();
    (void)t;
}

TEST(Flat, Void) {
    auto t = []() -> ca::Task<void> { co_return; }();
    ca::run(t);
}

TEST(Flat, Int) {
    auto t = []() -> ca::Task<int> { co_return 1; }();
    auto i = ca::run(t);
    ASSERT_EQ(i, 1);
}

