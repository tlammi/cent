/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include "cent/drv/http_session.hpp"
#include "cent/manifest.hpp"
#include "cent/manifest_list.hpp"
#include "cent/reference.hpp"

namespace cent {

/**
 * Container registry session
 *
 * This object takes a HTTP onnection which it uses to communicate with a
 * registry
 */
class RegistryClient {
 public:
    RegistryClient(drv::HttpSession* sess);

    ManifestList manifest_list(const Reference& img);

    Manifest manifest(const Reference& img);

    std::vector<uint8_t> blob(const Reference& img);

 private:
    drv::HttpSession* m_sess;
};
}  // namespace cent