/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include "cent/def.hpp"

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
};

std::unique_ptr<Context> default_context();
}  // namespace cent::drv