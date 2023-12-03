#pragma once

#include <cent/concepts.hpp>
#include <cent/dt/anon.hpp>
#include <cent/http/status.hpp>
#include <cent/str.hpp>
#include <string_view>

namespace cent::http {

class Session {
 public:
    virtual ~Session() = default;

    template <concepts::proto<bool(std::string_view)> F>
    void on_write(F&& f) {
        m_on_write = Anon(std::in_place_type<F>, std::forward<F>(f));
        on_write_impl(
            [](char* data, size_t size, size_t count, void* userdata) {
                auto* f = static_cast<F*>(userdata);
                auto success = (*f)(std::string_view(data, size * count));
                return success ? size * count : 0;
            },
            m_on_write.data());
    }

    template <std::same_as<std::nullptr_t> F>
    void on_write(F&& /*f*/) {
        m_on_write.reset();
        on_write_impl(nullptr, nullptr);
    }

    template <concepts::proto<bool(std::string_view, std::string_view)> F>
    void on_header(F&& f) {
        m_on_header = Anon(std::in_place_type_t<F>(std::forward<F>(f)));
        on_header_impl(
            [](char* data, size_t size, size_t count, void* userdata) {
                auto* f = static_cast<F*>(userdata);
                auto [l, r] =
                    cent::split_left(std::string_view(data, size * count), ':');
                auto success = (*f)(l, r);
                return success ? size * count : 0;
            },
            m_on_header.data());
    }

    template <std::same_as<std::nullptr_t> F>
    void on_header(F&& /*f*/) {
        m_on_header.reset();
        on_header_impl(nullptr, nullptr);
    }

    template <class Map>
    void set_headers(Map&& headers) {
        clear_headers_impl();
        for (auto& [k, v] : headers) { add_header_impl(k, v); }
        commit_headers_impl();
    }

    Result get(const std::string& url) { return get_impl(url.c_str()); }
    Result get(const char* url) { return get_impl(url); }

 private:
    virtual void on_write_impl(size_t (*cb)(char*, size_t, size_t, void*),
                               void*) = 0;
    virtual void on_header_impl(size_t (*cb)(char*, size_t, size_t, void*),
                                void*) = 0;

    virtual void clear_headers_impl() = 0;

    virtual void add_header_impl(std::string_view key,
                                 std::string_view val) = 0;

    virtual void commit_headers_impl() = 0;

    virtual Result get_impl(const char* url) = 0;

    cent::Anon m_on_write{};
    cent::Anon m_on_header{};
};
}  // namespace cent::http
