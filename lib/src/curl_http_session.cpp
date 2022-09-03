#include <curlpp/Easy.hpp>
#include <curlpp/Infos.hpp>
#include <curlpp/Options.hpp>

#include "cent/http_session.hpp"

namespace cent {

class CurlHttpSession final : public HttpSession {
 public:
    CurlHttpSession() {}
    void on_header(std::string_view field, std::string& value) override {}

    int get(std::string_view url) override {
        m_easy.setOpt(curlpp::options::Url(std::string(url)));
        m_easy.perform();
        return curlpp::infos::ResponseCode::get(m_easy);
    }

 private:
    curlpp::Easy m_easy{};
};

std::unique_ptr<HttpSession> default_http_session() {
    return std::make_unique<CurlHttpSession>();
}

}  // namespace cent