#include <curl/curl.h>

#include <cent/bits/net/headers.hpp>
#include <iostream>

namespace cent::net {
namespace {
curl_slist* cast(void* p) { return static_cast<curl_slist*>(p); }
template <class D>
curl_slist* cast(const std::unique_ptr<void, D>& p) {
    return cast(p.get());
}
}  // namespace

Headers::Headers(
    std::initializer_list<std::pair<std::string_view, std::string_view>> args) {
    for (const auto& [k, v] : args) {
        auto* ptr =
            curl_slist_append(cast(m_ptr), std::format("{}: {}", k, v).c_str());
        m_ptr.release();
        m_ptr.reset(ptr);
    }
}
Headers::Headers(std::initializer_list<CStr> args) {
    for (const auto& a : args) {
        auto* ptr = curl_slist_append(cast(m_ptr), a);
        m_ptr.release();
        m_ptr.reset(ptr);
    }
}

Headers::~Headers() {}

size_t Headers::size() const noexcept {
    size_t i = 0;
    const auto* ptr = cast(m_ptr.get());
    while (ptr) {
        ptr = ptr->next;
        ++i;
    }
    return i;
}

void Headers::Deleter::operator()(void* ptr) {
    std::cerr << reinterpret_cast<std::ptrdiff_t>(ptr) << '\n';
    curl_slist_free_all(cast(ptr));
}

}  // namespace cent::net
