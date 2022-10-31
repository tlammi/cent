/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include <cent/cent.hpp>
#include <cent/sandbox/driver.hpp>
#include <cerrno>
#include <fstream>
#include <iostream>

#include "cent/archive.hpp"
#include "cent/core/context.hpp"
#include "cent/core/storage.hpp"
#include "cent/core/workspace.hpp"
#include "cent/fs.hpp"
#include "cent/logs.hpp"
#include "cent/net.hpp"
#include "cent/net/http_client.hpp"
#include "cent/oci/reference.hpp"
#include "cent/oci/registry_client.hpp"
#include "cent/oci/runtime/bundle.hpp"
#include "cent/sandbox.hpp"

namespace cent {

class Cent::CentImpl {
 public:
    CentImpl(Config config) : m_ctx{std::move(config)} {}

    Result pull(std::string_view image_ref) {
        Storage storage{m_ctx.storage_path};
        oci::Reference image{std::string(image_ref)};
        net::HttpClient http_client{};
        oci::RegistryClient client{&http_client};
        std::stringstream ss;
        auto manifest_list = client.manifest_list(image);
        auto entry =
            manifest_list.platform_manifest(oci::Platform{"amd64", "linux"});
        if (!entry) {
            logs::fatal("Manifest list: ", manifest_list);
            raise("no entry: 'amd64'");
        }
        logs::trace("Manifest list: ", manifest_list);
        std::string manifest_img_ref{image.repo()};
        manifest_img_ref += "@";
        manifest_img_ref += entry->digest.str();
        oci::Reference manifest_image{manifest_img_ref};
        auto manifest = client.manifest(manifest_image);
        logs::trace("Manifest: ", manifest);
        for (const auto& layer : manifest.layers()) {
            if (storage.layer_exists(layer.digest)) {
                logs::debug("Layer '", layer.digest.str(),
                            "' already present. Skipping");
                continue;
            }
            std::string blob_img_ref{image.repo()};
            blob_img_ref += "@";
            blob_img_ref += layer.digest.str();
            oci::Reference blob_image{blob_img_ref};
            auto blob = client.blob(blob_image);
            auto stream = storage.write_layer(layer.digest);
            stream->write(
                static_cast<const char*>(static_cast<const void*>(blob.data())),
                blob.size());
        }
        if (!storage.config_exists(manifest.config().digest)) {
            std::string blob_img_ref{image.repo()};
            blob_img_ref += "@";
            blob_img_ref += manifest.config().digest.str();
            oci::Reference config_image{blob_img_ref};
            auto blob = client.blob(config_image);
            auto stream = storage.write_config(manifest.config().digest);
            stream->write(
                static_cast<const char*>(static_cast<const void*>(blob.data())),
                blob.size());
        } else {
            logs::debug("Config '", manifest.config().digest.str(),
                        "' already present. Skipping");
        }
        if (!storage.manifest_exists(manifest.digest())) {
            logs::trace("Storing manifest '", manifest.digest().str(), "'");
            (*storage.write_manifest(manifest.digest())) << manifest;
        } else {
            logs::debug("Manifest '", manifest.digest().str(),
                        "' already present. Skipping");
        }
        storage.store_image_name(manifest.digest(), image);

        return {0, "foo"};
    }

    Result image_list() {
        Storage storage{m_ctx.storage_path};
        for (const auto& ref : storage.list_images()) { logs::print(ref); }
        return {0, ""};
    }

    Result create(std::string_view image) {
        Storage storage{m_ctx.storage_path};
        Workspace wspace{m_ctx.workspace_path};
        logs::trace("Getting manifest digest");
        auto manifest_digest = storage.lookup_manifest(std::string{image});
        logs::debug("Found manifest digest: ", manifest_digest);
        oci::Manifest manifest{
            nlohmann::json::parse(*storage.read_manifest(manifest_digest))};
        logs::debug("Parsed manifest: ", manifest);

        auto sandbox = sandbox::sandbox();
        sandbox->set_uid_maps(m_ctx.uid_maps);
        sandbox->set_gid_maps(m_ctx.gid_maps);
        logs::debug("Walking through layers");
        std::vector<stdfs::path> extract_paths{};
        for (const auto& layer : manifest.layers()) {
            if (!wspace.layer_exists(layer.digest)) {
                logs::trace("layer '", layer.digest,
                            "' does not exist. Extracting");
                auto src = storage.layer_path(layer.digest);
                auto dst = wspace.create_layer(layer.digest);
                logs::trace("forking to a sandbox");
                sandbox->fork([&]() { archive::unpack(src, dst); });
            }
            extract_paths.push_back(wspace.layer_path(layer.digest));
        }
        sandbox->fork([&] {
            oci::runtime::Bundle bundle{"/tmp/asdf", extract_paths};
        });
        return Result{0, ""};
    }

    void system_reset() {
        fs::rm(m_ctx.workspace_path, true, true);
        fs::rm(m_ctx.storage_path, true, true);
    }

 private:
    Context m_ctx;
};

Cent::Cent(Config config) : m_impl{new CentImpl(std::move(config))} {}
Cent::~Cent() {}

Result Cent::pull(std::string_view image) { return m_impl->pull(image); }
Result Cent::image_list() { return m_impl->image_list(); }
Result Cent::create(std::string_view image) { return m_impl->create(image); }

void Cent::system_reset() { return m_impl->system_reset(); }
void init_defaults() {
    fs::set_driver(fs::builtin_driver("linux"));
    net::set_driver(net::builtin_driver("curl"));
    sandbox::set_driver(sandbox::builtin_driver("namespace"));
}
}  // namespace cent