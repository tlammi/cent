#pragma once

#include <string_view>

namespace cent {

class Driver {
 public:
    Driver() = default;
    virtual ~Driver(){};

    Driver(Driver&&) = default;
    Driver& operator=(Driver&&) = default;

    Driver(const Driver&) = delete;
    Driver& operator=(const Driver&) = delete;

    virtual std::string_view name() const noexcept = 0;

 private:
};
}  // namespace cent