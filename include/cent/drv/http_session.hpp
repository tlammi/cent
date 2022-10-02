/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <functional>
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

[[deprecated]] std::unique_ptr<HttpSession> default_http_session();

using HttpSessionFunc = std::function<std::unique_ptr<HttpSession>()>;
void register_http_session(std::string name, const HttpSessionFunc& func);
std::vector<std::string_view> list_http_sessions();

}  // namespace cent::drv
