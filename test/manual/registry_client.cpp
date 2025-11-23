#include <cent/dist/client.hpp>
#include <cent/dist/http/smart_session.hpp>
#include <cent/exception.hpp>
#include <print>
#include <rfl/json.hpp>

struct VoidStream final : public cent::dist::BlobStream {
    void on_chunk(std::span<const std::byte> data) override {}
};

struct TextStream final : public cent::dist::BlobStream {
    std::string str{};
    void on_chunk(std::span<const std::byte> data) override {
        str.append(reinterpret_cast<const char*>(data.data()), data.size());
    }
};

struct Consumer final : public cent::dist::PullConsumer {
    void on_manifest(cent::Manifest mfest) override {
        std::println("manifest: {}", rfl::json::write(mfest));
    }
    /**
     * \brief Consume the received image config
     * */
    void on_config(cent::ImgConfig cfg) override {
        std::println("config: {}", rfl::json::write(cfg));
    }

    cent::dist::BlobStream* get_layer_stream(std::string_view digest) override {
        assert(false);
    }
    void free_layer_stream(cent::dist::BlobStream* stream) override {
        assert(false);
    }
};

void run(int argc, char** argv) {
    if (argc != 3)
        cent::raise(cent::ErrorCode::MissingArgument,
                    "usage: {} manifest|layer|config|pull URL", argv[0]);

    auto cmd = std::string_view(argv[1]);
    auto pool = cent::dist::http::SimpleSessionPool();
    auto client = cent::dist::client(pool);
    if (cmd == "manifest") {
        auto url = cent::dist::manifest_url(std::string_view(argv[2]));
        auto res = client->manifest(url);
        std::fputs(res.c_str(), stdout);
    } else if (cmd == "layer") {
        auto url = cent::dist::blob_url(std::string_view(argv[2]));
        auto stream = VoidStream();
        client->blob(url, stream);
    } else if (cmd == "config") {
        auto url = cent::dist::blob_url(std::string_view(argv[2]));
        auto stream = TextStream();
        client->blob(url, stream);
        std::fputs(stream.str.c_str(), stdout);
    } else if (cmd == "pull") {
        auto nm = cent::NameView(argv[2]);
        auto consumer = Consumer();
        cent::dist::pull(*client, consumer, {.reference = nm});
    } else {
        cent::raise(cent::ErrorCode::InvalidArgument,
                    "usage: {} manifest|layer|config|pull URL", argv[0]);
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
