/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include "workspace.hpp"

#include "cent/fs.hpp"
#include "cent/logs.hpp"

namespace cent {
namespace {

constexpr std::string_view LAYER_DIR = "layers";
}  // namespace

Workspace::Workspace(const stdfs::path& root) : m_root{root} {
    fs::mkdir(m_root / LAYER_DIR, true);
}

stdfs::path Workspace::create_layer(DigestView digest) {
    auto path = m_root / LAYER_DIR / digest.algo() / digest.value();
    if (fs::exists(path)) raise("Layer already exists: ", digest);
    fs::mkdir(path, true);
    return path;
}
stdfs::path Workspace::layer_path(DigestView digest) {
    return m_root / LAYER_DIR / digest.algo() / digest.value();
}

bool Workspace::layer_exists(DigestView digest) {
    auto path = m_root / LAYER_DIR / digest.algo() / digest.value();
    logs::trace("Checking if ", path, " exists");
    return fs::exists(path);
}
}  // namespace cent