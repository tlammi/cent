#pragma once

#include "cent/drv/context.hpp"
#include "cent/drv/file_system.hpp"
#include "cent/drv/http_session.hpp"

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
};

}  // namespace cent::drv