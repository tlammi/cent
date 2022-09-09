#pragma once

#include "cent/interface.hpp"
#include "cent/manifest.hpp"
#include "cent/manifest_list.hpp"
#include "cent/reference.hpp"

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

    ManifestList manifest_list(const Reference& img);

    Manifest manifest(const Reference& img);

    std::vector<uint8_t> blob(const Reference& img);

 private:
    HttpSession* m_sess;
};
}  // namespace cent