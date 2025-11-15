#include <curl/curl.h>

#include <cent/dist/http/header_list.hpp>
#include <cent/error.hpp>
#include <cent/util/split.hpp>
#include <cent/util/strip.hpp>

namespace cent::dist::http {
namespace {

CStr get_val(HeaderList::iterator it) noexcept {
    auto [_unused, v] = util::split_first((*it).view(), ':');
    v = util::strip_front(v);
    return CStr(v, null_terminated);
}
}  // namespace

auto HeaderList::iterator::operator++() noexcept -> iterator& {
    m_ptr = m_ptr->next;
    return *this;
}

auto HeaderList::iterator::operator++(int) noexcept -> iterator {
    auto tmp = *this;
    m_ptr = m_ptr->next;
    return tmp;
}

auto HeaderList::iterator::operator*() const noexcept -> value_type {
    return m_ptr->data;
}

HeaderList::HeaderList(std::initializer_list<HeaderPair> hdrs) {
    for (auto [k, v] : hdrs) { add(k, v); }
}

HeaderList::HeaderList(std::initializer_list<CStr> hdrs) {
    for (auto h : hdrs) { add(h); }
}

HeaderList::~HeaderList() { clear(); }

auto HeaderList::find(std::string_view key) const -> iterator {
    return std::ranges::find_if(*this, [&](CStr value) {
        auto [k, v] = util::split_first(value.view(), ':');
        return k == key;
    });
}

size_t HeaderList::size() const noexcept {
    size_t count = 0;
    for (auto unused : *this) {
        (void)unused;
        ++count;
    }
    return count;
}

bool HeaderList::empty() const noexcept { return size() == 0; }

CStr HeaderList::at(std::string_view key) const {
    auto it = find(key);
    if (it == end()) raise(ErrorCode::Generic, "at({})", key);
    return get_val(it);
}

CStr HeaderList::get_or(std::string_view key,
                        CStr default_value) const noexcept {
    auto it = find(key);
    if (it == end()) return default_value;
    return get_val(it);
}

void HeaderList::add(CStr hdr) {
    auto* tmp = curl_slist_append(m_head, hdr.c_str());
    if (!tmp) raise(ErrorCode::Generic, "curl_slist_append");
    m_head = tmp;
}
void HeaderList::add(std::string_view key, std::string_view val) {
    auto str = std::format("{}: {}", key, val);
    add(str.c_str());
}

void HeaderList::clear() noexcept {
    curl_slist_free_all(m_head);
    m_head = nullptr;
}

size_t HeaderList::erase(std::string_view key) {
    auto new_list = HeaderList();
    size_t count = 0;
    for (auto value : *this) {
        auto [k, v] = util::split_first(value.view(), ':');
        if (key == k) {
            ++count;
            continue;
        }
        new_list.add(value);
    }
    *this = std::move(new_list);
    return count;
}

}  // namespace cent::dist::http
