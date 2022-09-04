#include <curlpp/Easy.hpp>
#include <curlpp/Infos.hpp>
#include <curlpp/Options.hpp>

#include "cent/http/header.hpp"
#include "cent/http_session.hpp"
#include "cent/logs.hpp"

namespace cent {

class CurlHttpSession final : public HttpSession {
 public:
    CurlHttpSession() { m_easy.setOpt(curlpp::options::Verbose(true)); }
    void on_header(std::string_view field, std::string& value) override {
        m_on_header.emplace(field, std::reference_wrapper(value));
    }
    void set_header_field(std::string_view field,
                          std::string_view value) override {
        std::stringstream ss{};
        ss << field << ": " << value;
        auto iter = m_headers.begin();
        const auto end = m_headers.end();
        while (iter != end) {
            http::HeaderView header{*iter};
            if (header.field_name() == field) {
                *iter = ss.str();
                return;
            }
            ++iter;
        }
        m_headers.push_back(ss.str());
    }
    virtual std::string_view get_body() override {
        return m_body.rdbuf()->view();
    }

    int get(std::string_view url) override {
        m_body = std::stringstream();
        m_easy.setOpt(curlpp::options::HttpHeader(m_headers));
        m_easy.setOpt(curlpp::options::Url(std::string(url)));
        m_easy.setOpt(curlpp::options::HeaderFunction([&](char* buf, size_t s,
                                                          size_t n) {
            if (auto header =
                    http::HeaderView::try_parse(std::string_view(buf, s * n))) {
                auto iter = m_on_header.find(std::string(header->field_name()));
                if (iter != m_on_header.end())
                    iter->second.get() = header->value();
            }
            return s * n;
        }));

        m_easy.setOpt(
            curlpp::options::WriteFunction([&](char* buf, size_t s, size_t n) {
                m_body << std::string_view(buf, s * n);
                return s * n;
            }));
        m_easy.perform();
        return curlpp::infos::ResponseCode::get(m_easy);
    }

 private:
    curlpp::Easy m_easy{};
    std::map<std::string, std::reference_wrapper<std::string>> m_on_header{};
    std::list<std::string> m_headers{};
    std::stringstream m_body{};
};

std::unique_ptr<HttpSession> default_http_session() {
    return std::make_unique<CurlHttpSession>();
}

}  // namespace cent