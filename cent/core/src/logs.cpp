#include <sys/wait.h>

#include <cent/core/logs.hpp>

#include "cent/core/static_str.hpp"

namespace cent {
namespace {

LogLevel g_lvl = LogLevel::Trace;  // NOLINT

constexpr StaticStr log_lvl_str(LogLevel lvl) noexcept {
    using enum LogLevel;
    switch (lvl) {
        case Trace: return "[TRAC] ";
        case Debug: return "[DEBU] ";
        case Info: return "[INFO] ";
        case Warn: return "[WARN] ";
        case Err: return "[ERRO] ";
    }
    return "[UNKN] ";
}
}  // namespace
void logs_init(LogLevel lvl) { g_lvl = lvl; }

LogLevel log_level() { return g_lvl; }

void logs_flush() { std::fflush(stderr); }

namespace logdetail {

void push_log(LogLevel lvl, std::string&& msg) {
    std::fputs(log_lvl_str(lvl), stderr);
    std::fputs(msg.c_str(), stderr);
    std::fputs("\n", stderr);
}

}  // namespace logdetail
}  // namespace cent
