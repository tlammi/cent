#include <gtest/gtest.h>

#include <cent/async.hpp>

namespace ca = cent::async;
using namespace std::literals::chrono_literals;

TEST(Sleep, For) {
    auto t = []() -> ca::Task<void> { co_await ca::sleep_for(0s); };
    ca::run(t());
}

TEST(Sleep, Until) {
    auto t = []() -> ca::Task<void> {
        co_await ca::sleep_until(cent::time::Clock::now());
    };
    ca::run(t());
}
