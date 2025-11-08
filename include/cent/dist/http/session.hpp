#pragma once

#include <span>

namespace cent::dist::http {

struct Progress {
    long download_total;
    long downloaded;
    long upload_total;
    long uploaded;
};

class SessionHandler {
 public:
    virtual bool on_receive_header(std::span<const char> data) noexcept = 0;
    virtual bool on_receive(std::span<const char> data) noexcept = 0;
    virtual bool on_send(std::span<char> data) noexcept = 0;

    virtual bool on_progress(const Progress& prog) noexcept {
        (void)prog;
        return true;
    }

 protected:
    constexpr ~SessionHandler() = default;
};

class AnySession {
 public:
 private:
};

class Session {
 public:
 private:
};

}  // namespace cent::dist::http
