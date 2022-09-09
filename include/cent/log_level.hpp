#pragma once

namespace cent {

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Note,
    Warn,
    Err,
    Fatal,
};
extern LogLevel LOG_LEVEL;
}