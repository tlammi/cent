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
