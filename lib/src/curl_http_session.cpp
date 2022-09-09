
#include <cpr/cpr.h>

#include "cent/http/header.hpp"
#include "cent/http_session.hpp"
#include "cent/logs.hpp"

namespace cent {

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

    virtual std::string_view get_body() override {
        return m_body.rdbuf()->view();
    }

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

        // m_easy.setOpt(curlpp::options::HttpHeader(m_headers));
        // m_easy.setOpt(curlpp::options::Url(std::string(url)));
        /*m_easy.setOpt(curlpp::options::HeaderFunction(
            [&](char* buf, size_t s, size_t n) { return s * n; }));
*/
        /*
                m_easy.setOpt(
                    curlpp::options::WriteFunction([&](char* buf, size_t s,
           size_t n) { m_body << std::string_view(buf, s * n); return s * n;
                    }));

        m_easy.perform();
        return curlpp::infos::ResponseCode::get(m_easy);
                    */
    }

 private:
    cpr::Session m_sess{};
    std::map<std::string, std::string, std::less<>> m_received_headers{};
    cpr::Header m_headers{};
    std::stringstream m_body{};
};

std::unique_ptr<HttpSession> default_http_session() {
    return std::make_unique<CurlHttpSession>();
}

}  // namespace cent