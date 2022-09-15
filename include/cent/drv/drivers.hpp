#pragma once

#include "cent/drv/file_system.hpp"
#include "cent/drv/http_session.hpp"

namespace cent::drv {

class Drivers {
 public:
    virtual ~Drivers() {}
    virtual std::unique_ptr<HttpSession> http_session() = 0;
    virtual FileSystem* file_system() = 0;
};

}  // namespace cent::drv