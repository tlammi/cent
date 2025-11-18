#include <cent/dist/http/smart_session.hpp>
#include <cent/dist/registry_client.hpp>
#include <cent/exception.hpp>
#include <print>

struct VoidLayerStream final : public cent::dist::LayerStream {
    void on_chunk(std::span<const std::byte> data) override {}
};

void run(int argc, char** argv) {
    if (argc != 3)
        cent::raise(cent::ErrorCode::MissingArgument,
                    "usage: {} manifest|layer URL", argv[0]);

    auto cmd = std::string_view(argv[1]);
    auto raw_sess = cent::dist::http::Session();
    auto smart_sess = cent::dist::http::SmartSession(raw_sess);
    auto client = cent::dist::RegistryClient(smart_sess);
    if (cmd == "manifest") {
        auto url = cent::dist::manifest_url(std::string_view(argv[2]));
        auto res = client.manifest(url);
        std::println("{}", rfl::json::write(res, true));
    } else if (cmd == "layer") {
        auto url = cent::dist::blob_url(std::string_view(argv[2]));
        auto stream = VoidLayerStream();
        client.layer(url, stream);
    } else {
        cent::raise(cent::ErrorCode::InvalidArgument,
                    "usage: {} manifest|layer URL", argv[0]);
    }
}

int main(int argc, char** argv) {
    try {
        run(argc, argv);
    } catch (const std::exception& ex) {
        std::println(stderr, "{}", ex.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
