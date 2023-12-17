#pragma once

#include <curl/curl.h>

#include <cent/bits/net/curl_slist.hpp>
#include <cent/bits/net/headers.hpp>
#include <cent/bits/net/progress.hpp>
#include <cent/bits/net/result.hpp>
#include <cent/concepts.hpp>
#include <cent/dt.hpp>
#include <cent/str.hpp>
#include <span>

namespace cent::net {
namespace session_detail {

template <concepts::proto<bool(std::string_view, std::string_view)> F>
size_t header_callback(char* data, size_t size, size_t count, void* userdata) {
    auto* ptr = static_cast<F*>(userdata);
    auto hdr = std::string_view(data, size * count);
    auto [field, val] = split_left(hdr, ':');
    field = strip(field);
    val = strip(val);
    return (*ptr)(field, val) ? size * count : 0;
}

template <concepts::proto<bool(std::string_view)> F>
size_t write_callback(char* data, size_t size, size_t count, void* userdata) {
    auto* ptr = static_cast<F*>(userdata);
    return (*ptr)(std::string_view(data, size * count)) ? size * count : 0;
}

template <concepts::proto<size_t(std::span<char>)> F>
size_t read_callback(char* data, size_t size, size_t count, void* userdata) {
    auto* ptr = static_cast<F*>(userdata);
    return (*ptr)(std::span<char>(data, size * count));
}

template <concepts::proto<bool()> F>
size_t progress_callback(void* userdata, curl_off_t dltotal, curl_off_t dlnow,
                         curl_off_t ultotal, curl_off_t ulnow) {
    auto* ptr = static_cast<F*>(userdata);
    return (*ptr)(Progress{dltotal, dlnow, ultotal, ulnow}) ? 0 : 1;
}

}  // namespace session_detail

/**
 * \brief A single networking session
 * */
class Session {
 public:
    Session();

    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    Session(Session&& other) noexcept;
    Session& operator=(Session&& other) noexcept;

    ~Session();

    template <concepts::proto<bool(std::string_view, std::string_view)> F>
    void on_header(F&& f) {
        m_on_header = Anon(std::in_place_type<std::remove_cvref_t<F>>,
                           std::forward<F>(f));
        using T = std::remove_cvref_t<F>;
        m_on_header = Anon(std::in_place_type<T>, std::forward<F>(f));
        on_header_impl(session_detail::header_callback<T>, m_on_header.data());
    }

    template <concepts::proto<bool(std::string_view)> F>
    void on_write(F&& f) {
        using T = std::remove_cvref_t<F>;
        m_on_write = Anon(std::in_place_type<T>, std::forward<F>(f));
        on_write_impl(session_detail::write_callback<T>, m_on_write.data());
    }

    template <concepts::proto<size_t(std::span<char>)> F>
    void on_read(F&& f) {
        using T = std::remove_cvref_t<F>;
        m_on_read = Anon(std::in_place_type<T>, std::forward<F>(f));
        on_read_impl(session_detail::read_callback<T>, m_on_read.data());
    }

    template <concepts::proto<bool(Progress)> F>
    void on_progress(F&& f) {
        using T = std::remove_cvref_t<F>;
        m_on_prog = Anon(std::in_place_type<T>, std::forward<F>(f));
        on_progress_impl(session_detail::progress_callback<T>,
                         m_on_prog.data());
    }

    void set_headers(const Headers& headers);

    void url(const char* s);

    net::Result get();

 private:
    void on_header_impl(size_t (*cb)(char*, size_t, size_t, void*),
                        void* userdata) noexcept;

    void on_write_impl(size_t (*cb)(char*, size_t, size_t, void*),
                       void* userdata) noexcept;

    void on_read_impl(size_t (*cb)(char*, size_t, size_t, void*),
                      void* userdata);

    void on_progress_impl(int (*cb)(void*, curl_off_t, curl_off_t, curl_off_t,
                                    curl_off_t),
                          void* userdata);

    CURL* m_c{};
    CurlSlist m_headers{};
    Anon m_on_header{};
    Anon m_on_write{};
    Anon m_on_read{};
    Anon m_on_prog{};
};
}  // namespace cent::net
