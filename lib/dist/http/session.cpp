#include <curl/curl.h>

#include <cent/dist/http/session.hpp>
#include <cent/error.hpp>
#include <ranges>

namespace cent::dist::http {
using namespace std::literals;
namespace {

#define CURL_SET(handle, opt, arg)                                       \
    do {                                                                 \
        if (curl_easy_setopt(handle, opt, arg) != CURLE_OK) [[unlikely]] \
            raise(ErrorCode::Generic, "curl_easy_setopt(" #opt ")");     \
    } while (false)

constexpr auto nth(auto&& range, size_t idx) {
    return *(range | std::views::drop(idx)).begin();
}

constexpr std::pair<std::string_view, std::string_view> split_header(
    std::string_view hdr) noexcept {
    auto idx = hdr.find(':');
    if (idx == std::string_view::npos) return {hdr, {}};
    return {hdr.substr(0, idx), hdr.substr(idx + 1)};
}

extern "C" {
size_t curl_header_fn(char* buf, size_t size, size_t nitems,
                      void* userdata) noexcept {
    static constexpr auto http_prefix = "HTTP/"sv;
    auto* ptr = static_cast<DataSink*>(userdata);
    auto view = std::string_view(buf, size * nitems);
    if (view.starts_with(http_prefix)) {
        view.remove_prefix(http_prefix.size());
        auto code = std::string_view(nth(view | std::views::split(' '), 1));
        http::StatusCode num_code{};
        auto res =
            std::from_chars(code.data(), code.data() + code.size(), num_code);
        (void)res;  // TODO: check
        if (!ptr->on_status(num_code)) return 0;
    } else {
        auto [key, val] = split_header(view);
        if (!ptr->on_header(key, val)) return 0;
    }
    return size * nitems;
}

size_t curl_write_fn(char* buf, size_t size, size_t nmemb,
                     void* userdata) noexcept {
    auto* ptr = static_cast<DataSink*>(userdata);
    auto view = std::string_view(buf, size * nmemb);
    return ptr->on_write(view) ? size * nmemb : 0;
}

size_t curl_read_fn(char* buffer, size_t size, size_t nitems, void* userdata) {
    auto* ptr = static_cast<DataSrc*>(userdata);
    auto buf = std::span<char>(buffer, size * nitems);
    auto res = ptr->on_read(buf);
    if (!res.data()) return CURL_READFUNC_ABORT;
    return res.size();
}
int curl_progress_fn(void* clientp, curl_off_t dltotal, curl_off_t dlnow,
                     curl_off_t ultotal, curl_off_t ulnow) {
    auto prog = Progress{
        .dltotal = dltotal,
        .dlnow = dlnow,
        .ultotal = ultotal,
        .ulnow = ulnow,
    };
    auto* ptr = static_cast<ProgressSink*>(clientp);
    return ptr->on_progress(prog) ? 0 : 1;
}

int curl_debug_fn(CURL* handle, curl_infotype type, char* data, size_t len,
                  void* clientp) noexcept {
    (void)handle;
    auto* ptr = static_cast<LogSink*>(clientp);
    auto view = std::string_view(data, len);
    switch (type) {
        case CURLINFO_TEXT: ptr->on_info(view); break;
        case CURLINFO_HEADER_IN: ptr->on_header_in(view); break;
        case CURLINFO_HEADER_OUT: ptr->on_header_out(view); break;
        default: break;
    }
    return 0;
}
}
}  // namespace

Session::Session() : m_handle(curl_easy_init()) {
    if (!m_handle) { raise(ErrorCode::Generic, "curl_easy_init()"); }
    CURL_SET(m_handle, CURLOPT_FOLLOWLOCATION, 1L);
}

Session::Session(Session&& other) noexcept
    : m_handle(std::exchange(other.m_handle, nullptr)),
      m_hdr_list(std::exchange(other.m_hdr_list, nullptr)) {}

Session& Session::operator=(Session&& other) noexcept {
    std::destroy_at(this);
    std::construct_at(this, std::move(other));
    return *this;
}

Session::~Session() {
    curl_easy_cleanup(m_handle);
    curl_slist_free_all(m_hdr_list);
}

void Session::data_sink(DataSink* sink) {
    if (!sink) {
        CURL_SET(m_handle, CURLOPT_HEADERFUNCTION, nullptr);
        CURL_SET(m_handle, CURLOPT_WRITEFUNCTION, nullptr);
    } else {
        CURL_SET(m_handle, CURLOPT_HEADERFUNCTION, &curl_header_fn);
        CURL_SET(m_handle, CURLOPT_WRITEFUNCTION, &curl_write_fn);
    }
    CURL_SET(m_handle, CURLOPT_HEADERDATA, sink);
    CURL_SET(m_handle, CURLOPT_WRITEDATA, sink);
}

void Session::data_src(DataSrc* src) {
    if (!src)
        CURL_SET(m_handle, CURLOPT_READFUNCTION, nullptr);
    else
        CURL_SET(m_handle, CURLOPT_READFUNCTION, &curl_read_fn);
    CURL_SET(m_handle, CURLOPT_READDATA, src);
}

void Session::progress_sink(ProgressSink* prog) {
    if (!prog)
        CURL_SET(m_handle, CURLOPT_XFERINFOFUNCTION, nullptr);
    else
        CURL_SET(m_handle, CURLOPT_XFERINFOFUNCTION, &curl_progress_fn);
    CURL_SET(m_handle, CURLOPT_XFERINFODATA, prog);
}
void Session::log_sink(LogSink* sink) {
    if (!sink)
        CURL_SET(m_handle, CURLOPT_DEBUGFUNCTION, nullptr);
    else
        CURL_SET(m_handle, CURLOPT_DEBUGFUNCTION, &curl_debug_fn);
    CURL_SET(m_handle, CURLOPT_DEBUGDATA, sink);
}

void Session::set_header(std::string_view key, std::string_view val) {
    auto hdr = std::format("{}: {}", key, val);
    auto* new_list = curl_slist_append(m_hdr_list, hdr.c_str());
    if (!new_list) raise(ErrorCode::Generic, "curl_slist_append()");
    m_hdr_list = new_list;
    CURL_SET(m_handle, CURLOPT_HTTPHEADER, m_hdr_list);
}

void Session::set_url(const Url& url) {
    CURL_SET(m_handle, CURLOPT_URL, url.c_str());
}

void Session::get() {
    CURL_SET(m_handle, CURLOPT_HTTPGET, 1);
    if (auto code = curl_easy_perform(m_handle); code != CURLE_OK)
        raise(ErrorCode::Generic, "curl_easy_perform(): {}",
              curl_easy_strerror(code));
}

}  // namespace cent::dist::http
