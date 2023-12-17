#include <cent/bits/net/curl_slist.hpp>
#include <cent/util.hpp>

namespace cent {

CurlSlist::CurlSlist(CurlSlist&& other) noexcept {
    std::swap(m_front, other.m_front);
    std::swap(m_back, other.m_back);
}

CurlSlist& CurlSlist::operator=(CurlSlist&& other) noexcept {
    auto tmp = CurlSlist(std::move(other));
    std::swap(m_front, tmp.m_front);
    std::swap(m_back, tmp.m_back);
    return *this;
}

CurlSlist::~CurlSlist() { curl_slist_free_all(m_front); }

void CurlSlist::append(const char* s) {
    auto* res = curl_slist_append(m_back, s);
    if (!res) panic("curl_slist_append()");
    if (!m_front) {
        m_front = res;
        m_back = res;
    } else {
        m_back = m_back->next;
    }
}
curl_slist* CurlSlist::handle() const noexcept { return m_front; }

void CurlSlist::clear() noexcept {
    curl_slist_free_all(m_front);
    m_front = nullptr;
    m_back = nullptr;
}

}  // namespace cent
