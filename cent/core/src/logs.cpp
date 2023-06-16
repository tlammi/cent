#include <cent/core/logs.hpp>

#include "cent/core/static_str.hpp"

namespace cent {
namespace {

StaticStr log_lvl_str(LogLevel lvl) {
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
namespace logsdetail {

void push_log(LogLevel lvl, std::string&& msg) {
    std::fputs(log_lvl_str(lvl), stderr);
    std::fputs(msg.c_str(), stderr);
    std::fputs("\n", stderr);
}

}  // namespace logsdetail
}  // namespace cent
