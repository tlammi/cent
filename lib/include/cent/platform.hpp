/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

namespace cent {

struct Platform {
    std::string architecture;
    std::string os;
    std::string variant;
};

}  // namespace cent