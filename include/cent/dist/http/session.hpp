#pragma once

#include <cent/dist/http/status.hpp>
#include <cent/dist/http/url.hpp>
#include <span>
#include <string_view>

using CURL = void;
struct curl_slist;

namespace cent::dist::http {

class DataSink {
 public:
    virtual bool on_status(StatusCode code) noexcept = 0;
    virtual bool on_header(std::string_view key,
                           std::string_view val) noexcept = 0;
    virtual bool on_write(std::string_view buf) noexcept = 0;

 protected:
    ~DataSink() = default;
};

class DataSrc {
 public:
    virtual bool on_read(std::span<char> buf) noexcept = 0;

 protected:
    ~DataSrc() = default;
};

struct Progress {
    long dltotal;
    long dlnow;
    long ultotal;
    long ulnow;
};

class ProgressSink {
 public:
    virtual bool on_progress(Progress progress) noexcept = 0;

 protected:
    ~ProgressSink() = default;
};

class Session {
 public:
    Session();
    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    Session(Session&& other) noexcept;
    Session& operator=(Session&& other) noexcept;
    ~Session();

    void data_sink(DataSink* sink);
    void data_src(DataSrc* src);
    void progress_sink(ProgressSink* prog);

    void set_header(std::string_view key, std::string_view val);

    void set_url(UrlView url);

    void get();

 private:
    CURL* m_handle;
    curl_slist* m_hdr_list{};
};

}  // namespace cent::dist::http
