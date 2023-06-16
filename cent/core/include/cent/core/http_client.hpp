#pragma once

#include <cctype>
#include <cent/core/memory.hpp>
#include <map>
#include <string>

namespace cent {
namespace httpdetail {

class CaseInsensitiveCmp {
 public:
    bool operator()(std::string_view l, std::string_view r) const noexcept {
        size_t end = std::min(l.size(), r.size());
        for (size_t i = 0; i < end; ++i) {
            auto il = std::tolower(l[i]);
            auto ir = std::tolower(r[i]);
            if (il < ir) return true;
        }
        if (l.size() < r.size()) return true;
        return false;
    }
};
}  // namespace httpdetail

using HttpHeader =
    std::map<std::string, std::string, httpdetail::CaseInsensitiveCmp>;

/**
 * \brief Response to an HTTP query
 * */
struct HttpResponse {
    int status_code;
    HttpHeader header;
    std::string body;
};

/**
 * \brief Object for building and performing HTTP queries
 * */
class HttpRequest {
 public:
    virtual ~HttpRequest() = default;

    HttpHeader& header() noexcept { return m_header; }
    const HttpHeader& header() const noexcept { return m_header; }
    std::string& body() noexcept { return m_body; }
    const std::string& body() const noexcept { return m_body; }

    virtual HttpResponse get(std::string_view url) = 0;

 private:
    HttpHeader m_header;
    std::string m_body;
};

Ptr<HttpRequest> http_request();

}  // namespace cent

