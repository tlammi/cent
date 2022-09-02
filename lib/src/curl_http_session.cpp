#include "cent/http_session.hpp"

namespace cent {

class CurlHttpSession final : public HttpSession {
 public:
    CurlHttpSession() {}

    int get(std::string_view url) override { return 200; }
};

std::unique_ptr<HttpSession> default_http_session() {
    return std::make_unique<CurlHttpSession>();
}

}  // namespace cent