#pragma once

#include <cent/dist/transport.hpp>

namespace cent::dist {

class RegistryClient {
 public:
    constexpr virtual ~RegistryClient() = default;
};

}  // namespace cent::dist
