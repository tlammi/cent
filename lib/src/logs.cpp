#include "cent/logs.hpp"

#include "cent.hpp"
#include "cent/type_traits.hpp"

namespace cent::logs {
namespace detail {

void push_log(LogMsg&& msg) {
    const auto curr = underlying_cast<LogLevel>(msg.log_level);
    const auto level = underlying_cast<LogLevel>(LOG_LEVEL);
    if (curr >= level) LOG_STREAM.get() << msg.ss.rdbuf() << '\n';
}
void do_print(std::stringbuf* buf) { OUT_STREAM.get() << buf << '\n'; }
}  // namespace detail
}  // namespace cent::logs