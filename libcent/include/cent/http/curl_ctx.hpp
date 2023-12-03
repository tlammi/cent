#pragma once

#include <curl/curl.h>

#include <cent/dt.hpp>

namespace cent::http {

class CurlCtx {
 public:
    CurlCtx();
    CurlCtx(CurlCtx const&) = delete;
    CurlCtx& operator=(CurlCtx const&) = delete;

    CurlCtx(CurlCtx&& other) noexcept;
    CurlCtx& operator=(CurlCtx&& other) noexcept;

    ~CurlCtx();

 private:
    Anon m_muts;
    CURLSH* m_c{};
};

}  // namespace cent::http
