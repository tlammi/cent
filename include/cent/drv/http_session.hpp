/* SPDX-License-Identifier:  GPL-3.0-or-later */
#pragma once

#include <memory>
#include <string_view>

namespace cent::drv {

class HttpSession {
 public:
    virtual ~HttpSession() {}

    virtual void capture_header_field(std::string_view field) = 0;

    virtual std::string& header_field(std::string_view field) = 0;
    virtual const std::string& header_field(std::string_view field) const = 0;

    virtual void set_header_field(std::string_view field,
                                  std::string_view value) = 0;

    virtual std::string_view get_body() = 0;

    virtual int get(std::string_view url) = 0;
};

std::unique_ptr<HttpSession> default_http_session();

}  // namespace cent
