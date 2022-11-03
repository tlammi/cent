#include "curl_driver.hpp"

#include <cpr/session.h>

#include <cent/cent.hpp>
#include <string_view>

#include "cent/raise.hpp"
#include "cent/util.hpp"
#include "http/header.hpp"
#include "http/token.hpp"

namespace cent::net {

class CurlHttpSession final : public HttpSession {
 public:
    CurlHttpSession() {
        if (LOG_LEVEL == LogLevel::Trace) m_sess.SetVerbose(true);
        m_sess.SetRedirect(cpr::Redirect(true));
    }
    void capture_header_field(std::string_view field) override {
        m_received_headers[std::string(field)] = "";
    }

    std::string& header_field(std::string_view field) override {
        auto iter = m_received_headers.find(field);
        if (iter != m_received_headers.end()) { return iter->second; }
        raise("Header not captured: ", field);
    }
    const std::string& header_field(std::string_view field) const override {
        auto iter = m_received_headers.find(field);
        if (iter != m_received_headers.end()) { return iter->second; }
        raise("Header not captured: ", field);
    }

    void set_header_field(std::string_view field,

                          std::string_view value) override {
        m_headers[std::string(field)] = std::string(value);
    }

    std::string_view get_body() override { return m_body.rdbuf()->view(); }

    int get(std::string_view url) override {
        m_body = std::stringstream();
        m_sess.SetOption(m_headers);
        m_sess.SetUrl(url);
        m_sess.SetHeaderCallback(cpr::HeaderCallback([&](std::string buf,
                                                         intptr_t /*unused*/) {
            if (auto header = http::HeaderView::try_parse(buf)) {
                auto iter =
                    m_received_headers.find(std::string(header->field_name()));
                if (iter != m_received_headers.end()) {
                    iter->second = header->value();
                }
            }
            return true;
        }));

        m_sess.SetWriteCallback(
            cpr::WriteCallback([&](std::string buf, intptr_t /*unused*/) {
                m_body << buf;
                return true;
            }));
        auto resp = m_sess.Get();
        return resp.status_code;
    }

 private:
    cpr::Session m_sess{};
    std::map<std::string, std::string, util::CaseInsensitiveCompare>
        m_received_headers{};
    cpr::Header m_headers{};
    std::stringstream m_body{};
};

class CurlDriver final : public Driver {
 public:
    std::unique_ptr<HttpSession> http_session() override {
        return std::make_unique<CurlHttpSession>();
    }

    std::string_view name() const noexcept override { return "curl"; }
};

std::shared_ptr<Driver> make_curl_driver() {
    return std::make_shared<CurlDriver>();
}
}  // namespace cent::net