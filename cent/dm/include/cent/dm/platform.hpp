/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <string>

namespace cent::dm {

struct Platform {
    std::string architecture;
    std::string os;
    std::string variant;

    bool operator==(const Platform& other) const noexcept {
        return architecture == other.architecture && os == other.os &&
               variant == other.variant;
    }

    bool operator!=(const Platform& other) const noexcept {
        return !(*this == other);
    }
};

}  // namespace cent::dm
