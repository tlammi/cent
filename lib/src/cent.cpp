/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include "cent.hpp"

#include <cerrno>
#include <fstream>
#include <iostream>

#include "cent/drv/file_system_impl.hpp"
#include "cent/http_client.hpp"
#include "cent/logs.hpp"
#include "cent/reference.hpp"
#include "cent/registry_client.hpp"
#include "cent/runtime/bundle.hpp"
#include "cent/storage.hpp"
#include "cent/workspace.hpp"

namespace cent {

LogLevel LOG_LEVEL{LogLevel::Info};

/**
 * Log consuming stream
 *
 * Set this to change where logs are written
 */
std::reference_wrapper<std::ostream> LOG_STREAM{std::cerr};

/**
 * Stdout stream
 */
std::reference_wrapper<std::ostream> OUT_STREAM{std::cout};

class Cent::CentImpl {
 public:
    CentImpl(drv::Drivers* drivers) : m_drivers{drivers} {}

    Result pull(std::string_view image_ref) {
        Storage storage{m_drivers->context()->storage_path()};
        Reference image{std::string(image_ref)};
        HttpClient http_client{m_drivers};
        RegistryClient client{&http_client};
        std::stringstream ss;
        auto manifest_list = client.manifest_list(image);
        auto entry =
            manifest_list.platform_manifest(Platform{"amd64", "linux"});
        if (!entry) {
            logs::fatal("Manifest list: ", manifest_list);
            raise("no entry: 'amd64'");
        }
        logs::trace("Manifest list: ", manifest_list);
        std::string manifest_img_ref{image.repo()};
        manifest_img_ref += "@";
        manifest_img_ref += entry->digest.str();
        Reference manifest_image{manifest_img_ref};
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
            Reference blob_image{blob_img_ref};
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
            Reference config_image{blob_img_ref};
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
        Storage storage{m_drivers->context()->storage_path()};
        for (const auto& ref : storage.list_images()) { logs::print(ref); }
        return {0, ""};
    }

    Result create(std::string_view image) {
        Storage storage{m_drivers->context()->storage_path()};
        Workspace wspace{m_drivers->context()->workspace_path()};
        logs::trace("Getting manifest digest");
        auto manifest_digest = storage.lookup_manifest(std::string{image});
        logs::debug("Found manifest digest: ", manifest_digest);
        Manifest manifest{
            nlohmann::json::parse(*storage.read_manifest(manifest_digest))};
        logs::debug("Parsed manifest: ", manifest);

        auto sandbox = m_drivers->sandbox();
        using IdMap = drv::Sandbox::IdMap;
        sandbox->set_uid_maps(m_drivers->context()->uid_maps());
        sandbox->set_gid_maps(m_drivers->context()->gid_maps());
        logs::debug("Walking through layers");
        std::vector<stdfs::path> extract_paths{};
        for (const auto& layer : manifest.layers()) {
            if (!wspace.layer_exists(layer.digest)) {
                logs::trace("layer '", layer.digest,
                            "' does not exist. Extracting");
                auto src = storage.layer_path(layer.digest);
                auto dst = wspace.create_layer(layer.digest);
                logs::trace("forking to a sandbox");
                sandbox->fork(
                    [&]() { m_drivers->unpacker()->unpack(src, dst); });
            }
            extract_paths.push_back(wspace.layer_path(layer.digest));
        }
        sandbox->fork([&] {
            runtime::Bundle bundle{m_drivers, "/tmp/asdf"};
            drv::fs().union_mount(extract_paths, bundle.root(), true);
        });
        return Result{0, ""};
    }

 private:
    drv::Drivers* m_drivers;
};

Cent::Cent(drv::Drivers* drivers) : m_impl{new CentImpl(drivers)} {}
Cent::~Cent() {}

Result Cent::pull(std::string_view image) { return m_impl->pull(image); }
Result Cent::image_list() { return m_impl->image_list(); }
Result Cent::create(std::string_view image) { return m_impl->create(image); }

}  // namespace cent