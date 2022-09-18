/* SPDX-License-Identifier:  GPL-3.0-or-later */
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