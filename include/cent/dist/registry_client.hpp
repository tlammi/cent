#pragma once

#include <cent/dist/transport.hpp>

namespace cent::dist {

struct Placeholder {};

class RegistryClient {
 public:
    constexpr virtual ~RegistryClient() = default;
};

}  // namespace cent::dist
