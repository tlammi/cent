#include "cent/logs.hpp"

#include "cent.hpp"

namespace cent::logs {
namespace detail {

void push_log(LogMsg&& msg) { LOG_STREAM.get() << msg.ss.rdbuf() << '\n'; }
}  // namespace detail
}  // namespace cent::logs