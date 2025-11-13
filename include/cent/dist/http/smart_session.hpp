#pragma once

#include <cent/dist/http/session.hpp>
#include <memory>

namespace cent::dist::http {

class SmartSession final : public AnySession {
 public:
    SmartSession(Session& sess);
    ~SmartSession();
    void data_sink(DataSink* sink) override;
    void data_src(DataSrc* src) override;

    void set_url(const Url& url) override;

    void get() override;

 private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
}  // namespace cent::dist::http
