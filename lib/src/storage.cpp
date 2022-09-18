/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include "cent/storage.hpp"

#include "cent/storage/images.hpp"

namespace cent {
namespace {
stdfs::path layerdir(const stdfs::path& root) { return root / "layers"; }

stdfs::path configdir(const stdfs::path& root) { return root / "configs"; }

stdfs::path manifestdir(const stdfs::path& root) { return root / "manifests"; }

stdfs::path lockfile(const stdfs::path& root) { return root / "cent.lock"; }
stdfs::path imagedb_file(const stdfs::path& root) {
    return root / "images.json";
}

stdfs::path layerpath(drv::FileSystem* fs, const stdfs::path& root,
                      DigestView digest) {
    auto p = layerdir(root) / digest.algo();
    fs->mkdir(p, true);
    return p / digest.value();
}

stdfs::path configpath(drv::FileSystem* fs, const stdfs::path& root,
                       DigestView digest) {
    auto p = configdir(root) / digest.algo();
    fs->mkdir(p, true);
    return p / digest.value();
}

stdfs::path manifestpath(drv::FileSystem* fs, const stdfs::path& root,
                         DigestView digest) {
    auto p = manifestdir(root) / digest.algo();
    fs->mkdir(p, true);
    return p / digest.value();
}
}  // namespace

Storage::Storage(drv::FileSystem* fs, const stdfs::path& root)
    : m_fs{fs}, m_root{root} {
    m_fs->mkdir(layerdir(m_root), true);
    m_fs->mkdir(configdir(m_root), true);
    m_fs->mkdir(manifestdir(m_root), true);
    m_lk = LockFile{m_fs, lockfile(m_root)};
}

bool Storage::layer_exists(DigestView digest) const {
    auto path = layerpath(m_fs, m_root, digest);
    return m_fs->exists(path);
}

bool Storage::config_exists(DigestView digest) const {
    auto path = configpath(m_fs, m_root, digest);
    return m_fs->exists(path);
}

bool Storage::manifest_exists(DigestView digest) const {
    auto path = manifestpath(m_fs, m_root, digest);
    return m_fs->exists(path);
}
std::unique_ptr<std::iostream> Storage::write_layer(DigestView digest) {
    auto path = layerpath(m_fs, m_root, digest);
    return m_fs->open_file(path, std::ios_base::out | std::ios_base::binary);
}

stdfs::path Storage::layer_path(DigestView digest) {
    return layerpath(m_fs, m_root, digest);
}

std::unique_ptr<std::iostream> Storage::read_layer(DigestView digest) {
    auto path = layerpath(m_fs, m_root, digest);
    return m_fs->open_file(path, std::ios_base::in | std::ios_base::binary);
}

std::unique_ptr<std::iostream> Storage::write_config(DigestView digest) {
    auto path = configpath(m_fs, m_root, digest);
    return m_fs->open_file(path, std::ios_base::out);
}

std::unique_ptr<std::iostream> Storage::read_config(DigestView digest) {
    auto path = configpath(m_fs, m_root, digest);
    return m_fs->open_file(path, std::ios_base::in);
}
std::unique_ptr<std::iostream> Storage::read_manifest(DigestView digest) {
    auto path = manifestpath(m_fs, m_root, digest);
    return m_fs->open_file(path, std::ios_base::in);
}
std::unique_ptr<std::iostream> Storage::write_manifest(DigestView digest) {
    auto path = manifestpath(m_fs, m_root, digest);
    return m_fs->open_file(path, std::ios_base::out);
}

Digest Storage::lookup_manifest(const Reference& image) {
    auto path = imagedb_file(m_root);
    auto data =
        nlohmann::json::parse(*m_fs->open_file(path, std::ios_base::in));
    storage::Images images{data};
    auto& entry = images[image];
    return entry.manifest_digest;
}

void Storage::store_image_name(DigestView manifest, const Reference& image) {
    auto path = imagedb_file(m_root);
    nlohmann::json data = nlohmann::json::array();
    if (m_fs->exists(path))
        data = nlohmann::json::parse(*m_fs->open_file(path, std::ios_base::in));

    storage::Images images{data};
    auto& entry = images[manifest];
    for (const auto& i : entry.image_names) {
        if (i.str() == image.str()) return;
    }
    entry.image_names.push_back(image);
    auto file = m_fs->open_file(path, std::ios_base::out);
    (*file) << images.json().dump();
}

std::vector<Reference> Storage::list_images() const {
    auto path = imagedb_file(m_root);
    nlohmann::json data = nlohmann::json::array();
    if (m_fs->exists(path))
        data = nlohmann::json::parse(*m_fs->open_file(path, std::ios_base::in));

    storage::Images images{data};
    const auto& entries = images.entries();
    std::vector<Reference> out{};
    // Guess the size so each manifest has two aliases
    out.reserve(entries.size() * 2);
    for (const auto& e : entries) {
        for (const auto& i : e.image_names) { out.push_back(i); }
    }
    return out;
}

}  // namespace cent