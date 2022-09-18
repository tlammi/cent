/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
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