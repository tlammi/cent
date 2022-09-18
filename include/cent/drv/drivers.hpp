/* SPDX-License-Identifier:  GPL-3.0-or-later */
#pragma once

#include "cent/drv/context.hpp"
#include "cent/drv/file_system.hpp"
#include "cent/drv/http_session.hpp"
#include "cent/drv/sandbox.hpp"
#include "cent/drv/unpacker.hpp"

namespace cent::drv {

/**
 * Wrapper class for passing all drivers to cent::Cent one time
 */
class Drivers {
 public:
    virtual ~Drivers() {}
    virtual std::unique_ptr<HttpSession> http_session() = 0;
    virtual FileSystem* file_system() = 0;
    virtual Context* context() = 0;

    virtual std::unique_ptr<Sandbox> sandbox() = 0;

    virtual Unpacker* unpacker() = 0;
};

}  // namespace cent::drv