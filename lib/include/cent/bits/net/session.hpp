#pragma once

#include <cent/bits/net/headers.hpp>

namespace cent::net {

class Session {
 public:
    Session& headers(const Headers& hdrs);

 private:
};

}  // namespace cent::net
