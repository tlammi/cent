/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include "cent/drv/http_session.hpp"
#include "cent/net.hpp"
#include "cent/oci/manifest.hpp"
#include "cent/oci/manifest_list.hpp"
#include "cent/oci/reference.hpp"
#include "cent/oci/resource.hpp"

namespace cent::oci {

/**
 * Container registry session
 *
 * This object takes a HTTP onnection which it uses to communicate with a
 * registry
 */
class RegistryClient {
 public:
    RegistryClient(net::HttpSession* sess);

    Resource get_resource(const Reference& ref);

    [[deprecated("use get_resource()")]] ManifestList manifest_list(
        const Reference& img);

    [[deprecated("use get_resource()")]] Manifest manifest(
        const Reference& img);

    std::vector<uint8_t> blob(const Reference& img);

 private:
    net::HttpSession* m_sess;
};
}  // namespace cent::oci