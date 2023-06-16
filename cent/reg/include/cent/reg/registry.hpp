#pragma once

#include <cent/core.hpp>

#include "cent/core/http_client.hpp"

namespace cent::reg {
class Registry {
 public:
    Registry(std::string_view url);
    explicit Registry(Ptr<HttpRequest> sess);
    const HttpRequest* active_request() const;

 private:
};

}  // namespace cent::reg
