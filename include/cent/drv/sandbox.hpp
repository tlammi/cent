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
    /// Map UID from sandbox to outside of the sandbox
    struct UidMap {
        uint64_t in;     ///< First UID in container to map
        uint64_t out;    ///< First UID outside of the container to map
        uint64_t count;  ///< Number of UIDs to map
    };

    virtual ~Sandbox() {}

    /// Set UID maps to apply on the following calls to fork()
    virtual void set_uid_maps(const std::vector<UidMap>& map) = 0;

    /// Get the current UID maps
    virtual const std::vector<UidMap>& uid_maps() const = 0;

    virtual void set_gid_maps(const std::vector<UidMap>& map) = 0;

    virtual const std::vector<UidMap>& gid_maps() const = 0;

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