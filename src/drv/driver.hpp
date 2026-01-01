#pragma once

#include <memory>
#include <string_view>

#include "drv/program_ctx.hpp"

namespace cent::drv {

/**
 * \brief Provides information and constructs a driver
 * */
template <class T>
class Meta {
 public:
    virtual ~Meta() = default;

    /**
     * \brief Driver name
     *
     * This can be used by cent to select what drivers to use
     * */
    virtual std::string_view name() const noexcept = 0;

    /**
     * \brief Check whether the driver is supported
     * */
    virtual bool supported(ProgramCtxView& ctx) const = 0;

    /**
     * \brief Create the driver
     * */
    virtual std::unique_ptr<T> create() const = 0;
};
}  // namespace cent::drv
