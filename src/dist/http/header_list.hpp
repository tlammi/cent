#pragma once

#include <cent/c_str.hpp>
#include <memory>
#include <utility>

extern "C" {
struct curl_slist;
}

namespace cent::dist::http {

class HeaderList {
 public:
    class iterator {
        friend HeaderList;

     public:
        using value_type = CStr;
        using difference_type = std::ptrdiff_t;

        constexpr bool operator==(std::default_sentinel_t) const noexcept {
            return m_ptr == nullptr;
        }
        constexpr bool operator==(iterator it) const noexcept {
            return m_ptr == it.m_ptr;
        }

        iterator& operator++() noexcept;
        iterator operator++(int) noexcept;

        value_type operator*() const noexcept;

        constexpr iterator() noexcept = default;

     private:
        constexpr explicit iterator(const curl_slist* ptr) : m_ptr(ptr) {}
        const curl_slist* m_ptr{};
    };

    constexpr HeaderList() noexcept = default;

    using HeaderPair = std::pair<std::string_view, std::string_view>;
    explicit HeaderList(std::initializer_list<HeaderPair> hdrs);
    explicit HeaderList(std::initializer_list<CStr> hdrs);

    HeaderList(const HeaderList&) = delete;
    HeaderList& operator=(const HeaderList&) = delete;

    constexpr HeaderList(HeaderList&& other) noexcept
        : m_head(std::exchange(other.m_head, nullptr)) {}

    constexpr HeaderList& operator=(HeaderList&& other) noexcept {
        std::destroy_at(this);
        std::construct_at(this, std::move(other));
        return *this;
    }

    ~HeaderList();

    constexpr iterator begin() const noexcept { return iterator{m_head}; }
    constexpr std::default_sentinel_t end() const noexcept {
        return std::default_sentinel;
    }

    iterator find(std::string_view key) const;

    size_t size() const noexcept;
    bool empty() const noexcept;

    CStr at(std::string_view key) const;

    CStr get_or(std::string_view key, CStr default_value) const noexcept;

    void add(CStr hdr);
    void add(std::string_view key, std::string_view val);

    void clear() noexcept;

    size_t erase(std::string_view key);

 private:
    curl_slist* m_head{};
};
}  // namespace cent::dist::http
