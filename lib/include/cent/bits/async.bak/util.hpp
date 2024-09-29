#pragma once

#include <cent/bits/async/task.hpp>
#include <cent/time.hpp>
namespace cent::async {

Task<void> sleep_for(time::Duration dur);
Task<void> sleep_until(time::Point tp);
Task<void> yield();

void launch(Task<void> task);

}  // namespace cent::async
