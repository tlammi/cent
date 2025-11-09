#include <cent/dist/http/session.hpp>
#include <print>

namespace http = cent::dist::http;

class DataSink final : public http::DataSink {
 public:
    bool on_status(http::StatusCode code) noexcept override {
        std::println(stderr, "status code: {}", code);
        return true;
    }
    bool on_header(std::string_view key,
                   std::string_view val) noexcept override {
        std::println(stderr, "HDR< {}: {}", key, val);
        return true;
    }
    bool on_write(std::string_view buf) noexcept override {
        std::println("{}", buf);
        return true;
    }
};

int main(int argc, char** argv) {
    auto sess = http::Session();
    auto data_sink = DataSink();
    sess.data_sink(&data_sink);
    sess.set_url(http::Url("http://www.google.com"));
    sess.get();
}
