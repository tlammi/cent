#pragma once

#include <curl/curl.h>

#include <cent/util.hpp>

namespace cent::http {

class CurlSList {
 public:
    class Iterator {
     public:
        Iterator(curl_slist* handle) : m_handle(handle) {}

        Iterator& operator++() {
            CENT_ASSERT(m_handle);
            m_handle = m_handle->next;
            return *this;
        }
        const char* operator*() const {
            CENT_ASSERT(m_handle);
            return m_handle->data;
        }

        bool operator==(const Iterator& other) const {
            return m_handle == other.m_handle;
        }
        bool operator!=(const Iterator& other) const {
            return m_handle != other.m_handle;
        }

     private:
        curl_slist* m_handle;
    };
    CurlSList() = default;

    CurlSList(const CurlSList& other);
    CurlSList& operator=(const CurlSList& other);

    CurlSList(CurlSList&& other) noexcept;
    CurlSList& operator=(CurlSList&& other) noexcept;

    ~CurlSList();

    void append(const char* s);

    void clear() noexcept;

    curl_slist* handle() noexcept;

    Iterator begin() const { return Iterator(m_handle); }
    Iterator end() const { return Iterator(nullptr); }

 private:
    curl_slist* m_handle{};
    curl_slist* m_back{};
};
}  // namespace cent::http
