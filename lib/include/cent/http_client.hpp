#pragma once

#include "cent/interface.hpp"

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
    HttpClient(Interface* iface);

    void capture_header_field(std::string_view field) override;

    std::string& header_field(std::string_view field) override;
    const std::string& header_field(std::string_view field) const override;

    void set_header_field(std::string_view field,
                          std::string_view value) override;
    std::string_view get_body() override;
    int get(std::string_view url) override;

 private:
    Interface* m_iface;
    std::unique_ptr<HttpSession> m_sess;
};
}  // namespace cent