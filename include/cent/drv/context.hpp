/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <vector>

#include "cent/def.hpp"
#include "cent/types.hpp"

namespace cent::drv {

/**
 * Program execution context
 *
 * Normally populated from
 * - CLI arguments
 * - environment variables
 * - config files
 */
class Context {
 public:
    virtual ~Context() {}

    virtual stdfs::path storage_path() = 0;
    virtual stdfs::path workspace_path() = 0;

    /// Get UID maps that should be used for sandboxing
    virtual std::vector<IdMap> uid_maps() = 0;
    /// Get GID maps that should be used for sandboxing
    virtual std::vector<IdMap> gid_maps() = 0;
};

std::unique_ptr<Context> default_context();
}  // namespace cent::drv