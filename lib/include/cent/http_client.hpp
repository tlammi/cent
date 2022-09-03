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

    int get(std::string_view url) override;

 private:
    HttpSession* m_sess;
};
}  // namespace cent