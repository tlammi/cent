#include <atomic>
#include <cent/logs/logs.hpp>

namespace cent::logs {
namespace {
std::atomic<Level> g_level;
std::unique_ptr<Logger> g_owned_logger;
std::atomic<Logger*> g_logger;
}  // namespace
namespace detail {

void push_log(Level lvl, std::string msg) {
    Logger* logger = g_logger;
    if (!logger) return;
    logger->log({std::move(msg), lvl});
}
}  // namespace detail

void init(Level lvl, std::unique_ptr<Logger> logger) {
    g_level = lvl;
    g_owned_logger = std::move(logger);
    g_logger = g_owned_logger.get();
}

Level level() { return g_level; }

}  // namespace cent::logs
