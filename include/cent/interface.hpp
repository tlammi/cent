#pragma once

#include "cent/http_session.hpp"

namespace cent {

class Interface {
 public:
    virtual ~Interface() {}

    virtual HttpSession* http_session() = 0;
};
}  // namespace cent