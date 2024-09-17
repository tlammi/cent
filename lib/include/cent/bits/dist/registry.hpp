#pragma once

#include <cent/bits/dist/manifest_list.hpp>
#include <cent/core.hpp>
#include <cent/net.hpp>
#include <cent/util.hpp>

namespace cent::dist {

class Registry : Abstract {
 public:
    virtual Result<ManifestList> fetch_manifest_list(net::UrlView url) = 0;
    virtual Result<Manifest> fetch_manifest(net::UrlView url) = 0;
};
}  // namespace cent::dist
