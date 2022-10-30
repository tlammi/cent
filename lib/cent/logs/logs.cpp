/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include "cent/logs.hpp"

#include <cent/cent.hpp>
#include <iostream>

#include "cent/type_traits.hpp"

namespace cent {

std::reference_wrapper<std::ostream> LOG_STREAM{std::cerr};
std::reference_wrapper<std::ostream> OUT_STREAM{std::cout};
LogLevel LOG_LEVEL{LogLevel::Info};

namespace logs {
namespace detail {

void push_log(LogMsg&& msg) {
    const auto curr = underlying_cast<LogLevel>(msg.log_level);
    const auto level = underlying_cast<LogLevel>(LOG_LEVEL);
    if (curr >= level) LOG_STREAM.get() << msg.ss.rdbuf() << '\n';
}
void do_print(std::stringbuf* buf) { OUT_STREAM.get() << buf << '\n'; }
}  // namespace detail
}  // namespace logs
}  // namespace cent