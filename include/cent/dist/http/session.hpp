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
    virtual std::span<char> on_read(std::span<char> buf) noexcept = 0;

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

class AnySession {
 public:
    virtual void data_sink(DataSink* sink) = 0;
    virtual void data_src(DataSrc* src) = 0;

 protected:
    ~AnySession() = default;
};

class Session final : public AnySession {
 public:
    Session();
    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    Session(Session&& other) noexcept;
    Session& operator=(Session&& other) noexcept;
    ~Session();

    void data_sink(DataSink* sink) override;
    void data_src(DataSrc* src) override;
    void progress_sink(ProgressSink* prog);

    void set_header(std::string_view key, std::string_view val);

    void set_url(const Url& url);

    void get();

 private:
    CURL* m_handle;
    curl_slist* m_hdr_list{};
};

}  // namespace cent::dist::http
