#pragma once

#include <memory>

#include "cent/file_system_api.hpp"
#include "cent/http_session.hpp"

namespace cent {

class Interface {
 public:
    virtual ~Interface() {}

    virtual std::unique_ptr<HttpSession> http_session() = 0;
    virtual FileSystemApi* file_system() = 0;
};
}  // namespace cent