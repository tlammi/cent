#pragma once

namespace cent::component {

/**
 * \brief Mount driver
 *
 * Driver used for preparing a container root file system.
 * */
class Mount {
 public:
    virtual ~Mount() = default;
};
}  // namespace cent::component
