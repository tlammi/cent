/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include "cent/drv/driver.hpp"
#include "cent/types.hpp"

namespace cent::sandbox {

/**
 * Driver providing sandbox for executing commands.
 *
 * Sandbox is typically a set of namespaces with related settings.
 */
class Sandbox {
 public:
    virtual ~Sandbox() {}

    /// Set UID maps to apply on the following calls to fork()
    virtual void set_uid_maps(std::vector<IdMap> map) = 0;

    /// Get the current UID maps
    virtual const std::vector<IdMap>& uid_maps() const = 0;

    /// Set GID maps to apply on the following calls to fork()
    virtual void set_gid_maps(std::vector<IdMap> map) = 0;

    /// Get the current GID maps
    virtual const std::vector<IdMap>& gid_maps() const = 0;

    /**
     * Run the given callable in a sandboxed environment.
     *
     * Normal execution could consist of e.g:
     *  - Setting up eventfd() for IPC
     *  - call to clone3() system call.
     *  - Setting up UID maps in the parent
     *  - Waiting for the eventfd in child
     *  - Signaling child from parent
     *  - Waiting for child to exit in parent.
     */
    virtual void fork(const std::function<void()>& func) = 0;
};

using SandboxFunc = std::function<std::unique_ptr<Sandbox>()>;
void register_sandbox(std::string name, const SandboxFunc& func);
std::vector<std::string_view> list_sandboxes();

}  // namespace cent::sandbox