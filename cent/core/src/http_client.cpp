#include <cpr/cpr.h>

#include <cent/core/http_client.hpp>

#include "cpr/session.h"

namespace cent {
namespace {
class CurlRequest final : public HttpRequest {
 public:
    HttpResponse get(std::string_view url) override {
        // TODO: reuse session
        cpr::Session s{};
        s.SetUrl(cpr::Url(url));
        s.SetBody(body());
        s.SetHeader(cpr::Header{header().begin(), header().end()});
        auto resp = s.Get();
        return HttpResponse{
            .status_code = static_cast<int>(resp.status_code),  // NOLINT
            .header = HttpHeader{std::make_move_iterator(resp.header.begin()),
                                 std::make_move_iterator(resp.header.end())},
            .body = resp.text,
        };
    }

 private:
};
}  // namespace

Ptr<HttpRequest> http_request() { return make_ptr<CurlRequest>(); }

}  // namespace cent
