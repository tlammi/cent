#pragma once

#include <memory>

#include "dist/http/session_pool.hpp"

namespace cent::dist::http {

class SmartSession final : public AnySession {
 public:
    explicit SmartSession(SessionPool& sess_pool);
    ~SmartSession();
    void data_sink(DataSink* sink) override;
    void data_src(DataSrc* src) override;

    void set_header(std::string_view key, std::string_view val) override;

    void set_url(const Url& url) override;

    void get() override;

 private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
}  // namespace cent::dist::http
