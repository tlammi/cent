#pragma once

#include <cent/core.hpp>
#include <cent/reg/manifest_list.hpp>

namespace cent::reg {

/**
 * Connection to a remote (or local) Docker/OCI registry
 * */
class Registry {
 public:
    /**
     * \brief Attach the client to the given URL
     * */
    explicit Registry(std::string base_url);
    Registry(std::string base_url, Ptr<HttpRequest> req);
    const HttpRequest* active_request() const;

    ManifestList manifest_list();

 private:
    std::string m_url;
    Ptr<HttpRequest> m_req;
};

}  // namespace cent::reg
