#pragma once

#include <memory>
#include <string_view>

namespace cent {

class HttpSession {
 public:
    virtual ~HttpSession() {}

    // TODO: Rename to on_header_field
    virtual void on_header(std::string_view field, std::string& value) = 0;
    virtual void set_header_field(std::string_view field,
                                  std::string_view value) = 0;

    virtual std::string_view get_body() = 0;

    virtual int get(std::string_view url) = 0;
};

std::unique_ptr<HttpSession> default_http_session();

}  // namespace cent
