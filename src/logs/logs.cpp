#include "cent/logs.hpp"

#include <cassert>

namespace cent::logs {

namespace {
Level g_lvl{Level::None};
void (*g_on_log)(Level, std::string_view);
}  // namespace
void init(Level lvl, void (*on_log)(Level lvl, std::string_view msg)) {
    g_on_log = on_log;
    g_lvl = lvl;
}
void set_level(Level lvl) noexcept { g_lvl = lvl; }
Level level() noexcept { return g_lvl; }

namespace detail {
void Logger::do_log(std::string_view s) const {
    assert(g_on_log && "Log level set without setting logger");
    g_on_log(m_lvl, s);
}
}  // namespace detail

}  // namespace cent::logs
