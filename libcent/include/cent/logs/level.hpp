#pragma once

namespace cent::logs {

enum class Level {
    Trace,  // debug WHERE execution is
    Debug,  // debug WHAT program is doing
    Info,   // more generic information
    Note,   // abnormal but not a warning
    Warn,   // abnormal but can be managed
    Err,    // abnormal and cannot be managed, but does not cause a crash
    Fatal,  // abnormal causing a crash
};

}
