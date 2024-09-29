#pragma once

#include <chrono>
namespace cent::time {

using Clock = std::chrono::steady_clock;
using Point = Clock::time_point;
using Duration = Clock::duration;

}  // namespace cent::time
