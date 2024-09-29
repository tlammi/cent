#include <gtest/gtest.h>

#include <cent/async.hpp>

namespace ca = cent::async;
TEST(Mutex, One) {
    ca::Mutex mut{};
    auto t = [](ca::Mutex* mut) -> ca::Task<void> { auto lk = co_await *mut; };
    ca::run(t(&mut));
}

TEST(Mutex, Locking) {
    ca::Mutex mut{};
    size_t counter = 0;
    auto a = [](ca::Mutex* mut, size_t* counter) -> ca::Task<void> {
        {
            auto lk = co_await *mut;
            *counter += 2;
            *counter *= 3;
        }
        co_await ca::yield();
        {
            auto lk = co_await *mut;
            *counter -= 1;
        }
    };

    auto b = [](ca::Mutex* mut, size_t* counter) -> ca::Task<void> {
        {
            auto lk = co_await *mut;
            EXPECT_EQ(*counter, 6);
        }
        co_await ca::yield();
        {
            auto lk = co_await *mut;
            EXPECT_EQ(*counter, 5);
        }
    };

    ca::run_all(a(&mut, &counter), b(&mut, &counter));
}
