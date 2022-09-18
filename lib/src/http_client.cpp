/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include "cent/http_client.hpp"

#include <string>

#include "cent/http/header.hpp"
#include "cent/http/token.hpp"
#include "cent/logs.hpp"
#include "cent/raise.hpp"
namespace cent {
namespace {

constexpr std::string_view strip(std::string_view what, char token) {
    while (!what.empty() && what.front() == token) what.remove_prefix(1);
    while (!what.empty() && what.back() == token) what.remove_suffix(1);
    return what;
}
}  // namespace

HttpClient::HttpClient(drv::Drivers* drivers)
    : m_drivers{drivers}, m_sess{m_drivers->http_session()} {
    m_sess->capture_header_field("www-authenticate");
}

void HttpClient::capture_header_field(std::string_view field) {
    m_sess->capture_header_field(field);
}

std::string& HttpClient::header_field(std::string_view field) {
    return m_sess->header_field(field);
}
const std::string& HttpClient::header_field(std::string_view field) const {
    return m_sess->header_field(field);
}

void HttpClient::set_header_field(std::string_view field,
                                  std::string_view value) {
    m_sess->set_header_field(field, value);
}

std::string_view HttpClient::get_body() { return m_sess->get_body(); }

int HttpClient::get(std::string_view url) {
    int res = m_sess->get(url);
    if (res >= 200 && res < 300) return res;
    if (res >= 400 && res < 500) {
        std::string auth_challenge = m_sess->header_field("www-authenticate");
        if (!auth_challenge.empty()) {
            logs::debug("Got challenge: \'", auth_challenge, '\'');
            http::HeaderView auth_challenge_header(auth_challenge);
            auto token_url = auth_challenge_header.sub_value("Bearer realm");
            auto service = auth_challenge_header.sub_value("service");
            auto scope = auth_challenge_header.sub_value("scope");
            token_url = strip(token_url, '"');
            service = strip(service, '"');
            scope = strip(scope, '"');
            auto token_url_str = std::string(token_url) +
                                 "?service=" + std::string(service) +
                                 "&scope=" + std::string(scope);
            auto challenge_session = m_drivers->http_session();
            res = challenge_session->get(token_url_str);
            if (res != 200)
                raise("Unsupported status code when accessing ", url, ": ",
                      res);
            auto body = challenge_session->get_body();
            auto token = http::Token::parse(body);
            if (!token) raise("Could not parse token from '", body, "'");
            logs::debug("token: ", token->token);
            std::string value = "Bearer " + token->token;
            m_sess->set_header_field("Authorization", value);
            return m_sess->get(url);
        }
    }
    raise("Unsupported status code '", res, '\'');
}

}  // namespace cent