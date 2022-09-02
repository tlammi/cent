#pragma once

#include <string_view>

namespace cent {

class LogConsumer {
 public:
    virtual ~LogConsumer() {}
    virtual std::ostream& debug() = 0;
    virtual std::ostream& info() = 0;
    virtual std::ostream& note() = 0;
    virtual std::ostream& warn() = 0;
    virtual std::ostream& err() = 0;
    virtual std::ostream& fatal() = 0;

 private:
};
}  // namespace cent