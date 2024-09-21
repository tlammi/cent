#include "cent/bits/async/executor.hpp"

#include <gtest/gtest.h>

#include <cent/async.hpp>

namespace ca = cent::async;

TEST(Init, Default) {
    auto e = ca::Executor();
    ASSERT_TRUE(e.done());
}

TEST(Init, Tasks) {
    auto a = [] -> ca::Task<void> { co_return; };
    auto e = ca::Executor(a());
    ASSERT_FALSE(e.done());
}

TEST(Run, One) {
    size_t counter = 0;
    auto a = [](size_t* counter) -> ca::Task<void> {
        ++*counter;
        co_return;
    };

    auto e = ca::Executor(a(&counter));
    e.run();
    ASSERT_EQ(counter, 1);
}

TEST(Run, Multiple) {
    size_t counter = 0;
    auto a = [](size_t* counter) -> ca::Task<void> {
        ++*counter;
        co_return;
    };

    auto e = ca::Executor(a(&counter), a(&counter), a(&counter));
    e.run();
    ASSERT_EQ(counter, 3);
}

TEST(Run, Nested) {
    size_t counter = 0;
    auto a = [](size_t* counter) -> ca::Task<void> {
        ++*counter;
        co_return;
    };
    auto b = [](size_t* counter, ca::Task<void> t) -> ca::Task<void> {
        ++*counter;
        co_await t;
    };

    auto e = ca::Executor(b(&counter, a(&counter)), b(&counter, a(&counter)));
    e.run();
    ASSERT_EQ(counter, 4);
}

