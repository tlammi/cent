#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

namespace cent::drv {

/**
 * Driver providing sandbox for executing commands.
 *
 * Sandbox is typically a set of namespaces with related settings.
 */
class Sandbox {
 public:
    /// Map UID/GID from sandbox to outside of the sandbox
    struct IdMap {
        uint64_t in;     ///< First UID/GID in container to map
        uint64_t out;    ///< First UID/GID outside of the container to map
        uint64_t count;  ///< Number of UIDs/GIDs to map
    };

    virtual ~Sandbox() {}

    /// Set UID maps to apply on the following calls to fork()
    virtual void set_uid_maps(const std::vector<IdMap>& map) = 0;

    /// Get the current UID maps
    virtual const std::vector<IdMap>& uid_maps() const = 0;

    /// Set GID maps to apply on the following calls to fork()
    virtual void set_gid_maps(const std::vector<IdMap>& map) = 0;

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

std::unique_ptr<Sandbox> default_sandbox();
}  // namespace cent::drv