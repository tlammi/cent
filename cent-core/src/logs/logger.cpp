#include <cent/core/bits/logs/logger.hpp>

namespace cent::core::logs {
namespace {
// NOLINTNEXTLINE
void (*g_consumer)(Level, std::string) = nullptr;
// NOLINTNEXTLINE
Level g_level{Level::Trace};
}  // namespace

void init(void (*consumer)(Level, std::string)) { g_consumer = consumer; }
void set_level(Level level) { g_level = level; }
Level active_level() { return g_level; }

namespace detail {
void do_push_log(Level lvl, std::string s) {
    if (g_consumer) g_consumer(lvl, std::move(s));
}
}  // namespace detail
}  // namespace cent::core::logs
