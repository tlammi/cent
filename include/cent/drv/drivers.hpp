/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
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
class [[deprecated]] Drivers {
 public:
    virtual ~Drivers() {}
    virtual Context* context() = 0;

    virtual Unpacker* unpacker() = 0;
};

}  // namespace cent::drv