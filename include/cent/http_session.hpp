#pragma once

#include <memory>
#include <string_view>

namespace cent {

class HttpSession {
 public:
    virtual ~HttpSession() {}
    virtual int get(std::string_view url) = 0;
};

std::unique_ptr<HttpSession> default_http_session();

}  // namespace cent
