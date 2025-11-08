/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <cstdint>

namespace cent {

/// Map UID/GID from sandbox to outside of the sandbox
struct IdMap {
    uint64_t in;     ///< First UID/GID in container to map
    uint64_t out;    ///< First UID/GID outside of the container to map
    uint64_t count;  ///< Number of UIDs/GIDs to map
};

}  // namespace cent