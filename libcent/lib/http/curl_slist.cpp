#include <cent/http/curl_slist.hpp>
#include <cent/util.hpp>

namespace cent::http {

CurlSList::~CurlSList() { clear(); }

CurlSList::CurlSList(const CurlSList& other) {
    for (const auto& s : other) { append(s); }
}

CurlSList& CurlSList::operator=(const CurlSList& other) {
    if (this != &other) {
        clear();
        for (const auto& s : other) { append(s); }
    }
    return *this;
}

CurlSList::CurlSList(CurlSList&& other) noexcept {
    std::swap(m_handle, other.m_handle);
}

CurlSList& CurlSList::operator=(CurlSList&& other) noexcept {
    if (this != &other) {
        auto tmp = CurlSList(std::move(other));
        std::swap(m_handle, tmp.m_handle);
    }
    return *this;
}

void CurlSList::append(const char* s) {
    auto* res = curl_slist_append(nullptr, s);
    if (!res) panic("curl_slist_append()");
    m_handle = res;
}

void CurlSList::clear() noexcept {
    curl_slist_free_all(m_handle);
    m_handle = nullptr;
}

}  // namespace cent::http
