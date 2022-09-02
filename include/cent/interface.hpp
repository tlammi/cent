#pragma once

#include "cent/http_session.hpp"
#include "cent/log_consumer.hpp"

namespace cent {

class Interface {
 public:
    virtual ~Interface() {}

    virtual HttpSession* http_session() = 0;
    virtual LogConsumer* log_consumer() = 0;
};
}  // namespace cent