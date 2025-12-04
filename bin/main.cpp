/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */

#include <cent/dist/client.hpp>
#include <cent/name.hpp>
#include <cent/strg/storage.hpp>
#include <cstdlib>
#include <variant>

#include "cli.hpp"

namespace cent::bin {
namespace {

struct BlobStream final : public dist::BlobStream {
    void on_chunk(std::span<const std::byte> data) override {}
};

struct PullConsumer final : dist::PullConsumer {
    strg::Storage* store;
    std::list<BlobStream> streams{};

    explicit PullConsumer(strg::Storage* s) : store(s) {}

    void on_manifest(std::string_view mfest) override {
        store->set_manifest("foo", {});
    }

    void on_config(std::string_view cfg) override {
        store->set_config("bar", {});
    }

    dist::BlobStream* get_layer_stream(std::string_view digest) override {
        streams.emplace_back();
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
        auto store = strg::create_storage("/home/tlammi/.local/share/cent/");
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
