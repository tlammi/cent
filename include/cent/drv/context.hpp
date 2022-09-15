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
};

std::unique_ptr<Context> default_context();
}  // namespace cent::drv