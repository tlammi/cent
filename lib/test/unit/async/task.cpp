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
    ca::run(std::move(t));
}

TEST(Flat, Int) {
    auto t = [] -> ca::Task<int> { co_return 1; }();
    auto i = ca::run(std::move(t));
    ASSERT_EQ(i, 1);
}

TEST(Nested, Void) {
    auto a = [] -> ca::Task<void> { co_return; };
    auto b = [](ca::Task<void> t) -> ca::Task<void> {
        co_await t;
        co_return;
    }(a());
    ca::run(std::move(b));
}

TEST(Nested, Int) {
    auto a = [] -> ca::Task<int> { co_return 1; };
    auto b = [](ca::Task<int> t) -> ca::Task<int> {
        auto v = co_await t;
        co_return 2 + v;
    }(a());
    auto res = ca::run(std::move(b));
    ASSERT_EQ(res, 3);
}

TEST(Launch, One) {
    size_t counter = 0;
    auto root = [](size_t* counter) -> ca::Task<void> {
        auto child = [](size_t* counter) -> ca::Task<void> {
            ++*counter;
            co_return;
        };
        co_await ca::launch(child(counter));
    }(&counter);
    ca::run(std::move(root));
    ASSERT_EQ(counter, 1);
}

TEST(Launch, Multiple) {
    size_t counter = 0;
    auto root = [](size_t* counter) -> ca::Task<void> {
        auto child = [](size_t* counter) -> ca::Task<void> {
            ++*counter;
            co_return;
        };
        co_await ca::launch(child(counter));
        co_await ca::launch(child(counter));
        co_await ca::launch(child(counter));
        co_await ca::launch(child(counter));
    };
    ca::run(root(&counter));
    ASSERT_EQ(counter, 4);
}

