#pragma once

#include "cent/http_session.hpp"

namespace cent {

/**
 * Smart HTTP session
 *
 * A class type on top of a normal HttpSession which can recover from certain
 * HTTP errors, such as authentication challenges.
 *
 */
class HttpClient final : public HttpSession {
 public:
    HttpClient(HttpSession* sess);

    void on_header(std::string_view field, std::string& value) override;

    void set_header_field(std::string_view field,
                          std::string_view value) override;
    std::string_view get_body() override;
    int get(std::string_view url) override;

 private:
    HttpSession* m_sess;
};
}  // namespace cent