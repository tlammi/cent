#include <cent/bits/async/sleep.hpp>

namespace cent::async {

Sleeper sleep_for(time::Duration dur) {
    return sleep_until(time::Clock::now() + dur);
}

Sleeper sleep_until(time::Point tp) { return Sleeper(tp); }

}  // namespace cent::async
