#include <curlpp/Easy.hpp>
#include <curlpp/Infos.hpp>
#include <curlpp/Options.hpp>

#include "cent/http/header.hpp"
#include "cent/http_session.hpp"
#include "cent/logs.hpp"

namespace cent {

class CurlHttpSession final : public HttpSession {
 public:
    CurlHttpSession() {}
    void on_header(std::string_view field, std::string& value) override {
        m_on_header.emplace(field, std::reference_wrapper(value));
    }

    int get(std::string_view url) override {
        m_easy.setOpt(curlpp::options::Url(std::string(url)));
        m_easy.setOpt(curlpp::options::HeaderFunction([&](char* buf, size_t s,
                                                          size_t n) {
            if (auto header =
                    http::HeaderView::try_parse(std::string_view(buf, s * n))) {
                auto iter = m_on_header.find(header->field_name());
                if (iter != m_on_header.end())
                    iter->second.get() = header->value();
            }
            return s * n;
        }));
        m_easy.perform();
        return curlpp::infos::ResponseCode::get(m_easy);
    }

 private:
    curlpp::Easy m_easy{};
    std::map<std::string_view, std::reference_wrapper<std::string>>
        m_on_header{};
};

std::unique_ptr<HttpSession> default_http_session() {
    return std::make_unique<CurlHttpSession>();
}

}  // namespace cent