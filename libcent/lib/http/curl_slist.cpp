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
    std::swap(m_back, other.m_back);
}

CurlSList& CurlSList::operator=(CurlSList&& other) noexcept {
    if (this != &other) {
        auto tmp = CurlSList(std::move(other));
        std::swap(m_handle, tmp.m_handle);
        std::swap(m_back, tmp.m_back);
    }
    return *this;
}

void CurlSList::append(const char* s) {
    auto* res = curl_slist_append(m_back, s);
    if (!res) panic("curl_slist_append()");
    m_handle = res;
    if (m_back == nullptr) {
        m_back = m_handle;
    } else
        m_back = m_back->next;
}

void CurlSList::clear() noexcept {
    curl_slist_free_all(m_handle);
    m_handle = nullptr;
    m_back = nullptr;
}

}  // namespace cent::http
