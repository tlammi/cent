#pragma once

#include <string_view>

namespace cent::component {

class AuxCmd {
 public:
    virtual std::string_view name() const noexcept = 0;

    virtual int run(int argc, char** argv) const = 0;

 protected:
    ~AuxCmd() = default;
};
}  // namespace cent::component
