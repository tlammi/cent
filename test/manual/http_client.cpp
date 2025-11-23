#include <cent/dist/http/smart_session.hpp>
#include <print>

namespace dist = cent::dist;
namespace http = cent::dist::http;

class DataSink final : public http::DataSink {
 public:
    std::string buffer{};
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
        buffer.append(buf);
        return true;
    }
};

int main(int argc, char** argv) {
    auto pool = http::SimpleSessionPool();
    auto sess = http::SmartSession(pool);
    auto data_sink = DataSink();
    sess.data_sink(&data_sink);
    if (argc != 2) exit(1);
    sess.set_url(dist::Url(argv[1]));
    sess.get();
    std::println("{}", data_sink.buffer);
}
