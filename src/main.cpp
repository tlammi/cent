/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */

#include <cstdlib>
#include <variant>

#include "cli.hpp"
#include "dist/client.hpp"
#include "module.hpp"
#include "name.hpp"
#include "strg/storage.hpp"

namespace cent::bin {
namespace {

struct BlobStream final : public dist::BlobStream {
    strg::LayerOut out;
    explicit BlobStream(strg::LayerOut out) : out(std::move(out)) {}

    void on_chunk(std::span<const std::byte> data) override { out << data; }
};

struct PullConsumer final : dist::PullConsumer {
    strg::Storage* store;
    std::list<BlobStream> streams{};

    explicit PullConsumer(strg::Storage* s) : store(s) {}

    void on_manifest(std::string_view digest, std::string_view mfest) override {
        store->set_manifest(digest, mfest);
    }

    void on_config(std::string_view digest, std::string_view cfg) override {
        store->set_config(digest, cfg);
    }

    dist::BlobStream* get_layer_stream(std::string_view digest) override {
        streams.emplace_back(store->layers().write(digest, 1024 * 1024 * 10));
        return &streams.back();
    }
    void free_layer_stream(dist::BlobStream* stream) override {
        auto it = std::ranges::find_if(streams, [&](const auto& i) {
            return std::addressof(i) == stream;
        });
        assert(it != streams.end());
        streams.erase(it);
    }
};

struct Visitor {
    Cli* args;
    int operator()(Pull& p) {
        auto path =
            std::filesystem::path(std::getenv("HOME")) / ".local/share/cent/";
        auto store = strg::create_storage(path);
        auto client = dist::client();
        auto nm = cent::NameView(p.image);
        auto consumer = PullConsumer{store.get()};
        cent::dist::pull(*client, consumer, {.reference = nm});
        return EXIT_SUCCESS;
    }
};

int run(int argc, char** argv) {
    auto args = parse_cli(argc, argv);
    auto visitor = Visitor(&args);
    return std::visit(visitor, args.cmd);
}
}  // namespace
}  // namespace cent::bin
int main(int argc, char** argv) { return cent::bin::run(argc, argv); }
