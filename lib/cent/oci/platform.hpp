/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <string>

namespace cent::oci {

struct Platform {
    std::string architecture;
    std::string os;
    std::string variant;
};

}  // namespace cent