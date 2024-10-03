#include <gtest/gtest.h>

#include <cent/async.hpp>

namespace ca = cent::async;

TEST(Yield, One) {
    size_t counter = 0;
    auto t = [](size_t* counter) -> ca::Task<void> {
        ++*counter;
        co_await ca::yield();
        ++*counter;
    }(&counter);
    ca::Executor e{std::move(t)};
    while (counter < 1) e.resume();
    e.resume();
    ASSERT_EQ(counter, 2);
    ASSERT_TRUE(e.done());
}

TEST(Yield, Nested) {
    size_t counter = 0;
    auto parent = [](size_t& counter) -> ca::Task<void> {
        auto child = [](size_t& counter) -> ca::Task<void> {
            ++counter;
            std::println("before");
            co_await ca::yield();
            std::println("after");
            ++counter;
        };

        co_await child(counter);
        co_await child(counter);
        co_await child(counter);
        co_await child(counter);
    };
    ca::run(parent(counter));
    ASSERT_EQ(counter, 8);
}
