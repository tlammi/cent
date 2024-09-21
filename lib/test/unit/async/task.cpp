#include <gtest/gtest.h>

#include <cent/async.hpp>

namespace ca = cent::async;

TEST(Init, Void) {
    auto t = [] -> ca::Task<> { co_return; }();
    (void)t;
}

TEST(Init, Int) {
    auto t = [] -> ca::Task<int> { co_return 1; }();
    (void)t;
}

TEST(Flat, Void) {
    auto t = [] -> ca::Task<void> { co_return; }();
    ca::run(t);
}

TEST(Flat, Int) {
    auto t = [] -> ca::Task<int> { co_return 1; }();
    auto i = ca::run(t);
    ASSERT_EQ(i, 1);
}

TEST(Nested, Void) {
    auto a = [] -> ca::Task<void> { co_return; };
    auto b = [](ca::Task<void> t) -> ca::Task<void> {
        co_await t;
        co_return;
    }(a());
    ca::run(b);
}

TEST(Nested, Int) {
    auto a = [] -> ca::Task<int> { co_return 1; };
    auto b = [](ca::Task<int> t) -> ca::Task<int> {
        auto v = co_await t;
        co_return 2 + v;
    }(a());
    auto res = ca::run(b);
    ASSERT_EQ(res, 3);
}

