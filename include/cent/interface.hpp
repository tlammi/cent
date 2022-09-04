#pragma once

#include <memory>

#include "cent/http_session.hpp"

namespace cent {

class Interface {
 public:
    virtual ~Interface() {}

    virtual std::unique_ptr<HttpSession> http_session() = 0;
};
}  // namespace cent