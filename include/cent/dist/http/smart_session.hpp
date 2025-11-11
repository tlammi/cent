#pragma once

#include <cent/dist/http/session.hpp>
#include <memory>

namespace cent::dist::http {

class SmartSession {
 public:
    SmartSession();
    ~SmartSession();
    void data_sink(DataSink* sink);
    void data_src(DataSrc* src);
    void progress_sink(ProgressSink* prog);

    void set_url(const Url& url);

    void get();

 private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
}  // namespace cent::dist::http
