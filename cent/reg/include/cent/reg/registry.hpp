#pragma once

#include <cent/core.hpp>

namespace cent::reg {
class Registry {
 public:
    explicit Registry(Ptr<HttpSession> sess);
    const HttpSession* session() const;

 private:
};

}  // namespace cent::reg
