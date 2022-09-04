#pragma once

#include "cent/image.hpp"
#include "cent/interface.hpp"

namespace cent {

/**
 * Container registry session
 *
 * This object takes a HTTP onnection which it uses to communicate with a
 * registry
 */
class RegistryClient {
 public:
    RegistryClient(HttpSession* sess);

    std::string manifest(const Image& img);

 private:
    HttpSession* m_sess;
};
}  // namespace cent