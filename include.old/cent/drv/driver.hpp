#pragma once

namespace cent::drv {

/**
 * Base class for drivers
 *
 */
class Driver {
 public:
    Driver() = default;
    Driver(const Driver&) = delete;
    Driver& operator=(const Driver&) = delete;

    Driver(Driver&&) = default;
    Driver& operator=(Driver&&) = default;

    virtual ~Driver() {}
};
}  // namespace cent::drv