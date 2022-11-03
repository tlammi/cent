/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */

#include "registry_client.hpp"

#include <array>
#include <sstream>

#include "cent/logs.hpp"
#include "cent/raise.hpp"
#include "cent/util.hpp"
#include "media_type.hpp"

namespace cent::oci {

namespace {

void get_accept(std::stringstream& ss) {
    for (const auto& mtype : all_media_types()) { ss << ", " << mtype.mime; }
}

std::string registry_root_url(std::string_view registry, bool use_ssl) {
    std::stringstream ss{};
    ss << "http";
    if (use_ssl) ss << "s";
    ss << "://";
    if (registry == "docker.io") ss << "registry-1.";
    ss << registry << "/v2/";
    return ss.str();
}

std::string manifest_url(const Reference& img, bool use_ssl) {
    auto registry = img.registry();
    auto name = img.name();
    auto ref = img.digest().str();
    if (ref == "") ref = img.tag();
    std::stringstream url{};
    url << registry_root_url(registry, use_ssl);
    url << name << "/manifests/" << ref;
    return url.str();
}

std::string blob_url(const Reference& img, bool use_ssl) {
    auto registry = img.registry();
    auto name = img.name();
    auto ref = img.digest().str();
    if (ref == "") ref = img.tag();
    std::stringstream url{};
    url << registry_root_url(registry, use_ssl);
    url << name << "/blobs/" << ref;
    return url.str();
}
}  // namespace

RegistryClient::RegistryClient(net::HttpSession* sess) : m_sess{sess} {
    m_sess->capture_header_field("docker-distribution-api-version");
    m_sess->capture_header_field("docker-content-digest");
}

Resource RegistryClient::get_resource(const Reference& ref) {
    bool use_ssl = true;
    int status_code = m_sess->get(registry_root_url(ref.registry(), use_ssl));
    if (status_code == 0) {
        logs::warn("Disabling SSL");
        use_ssl = false;
        status_code = m_sess->get(registry_root_url(ref.registry(), use_ssl));
    }
    if (status_code != 200) raise("Failure status code: ", status_code);

    m_sess->set_header_field(
        "Accept",
        util::join_inline(", ", media_mime(MediaKind::DockerContainerImageV1),
                          media_mime(MediaKind::DockerManifestV2),
                          media_mime(MediaKind::DockerManifestListV2)));
    auto url = manifest_url(ref, use_ssl);
    logs::debug("url: ", url);
    status_code = m_sess->get(url);
    auto body = m_sess->get_body();
    logs::trace(body);
    auto json_body = nlohmann::json::parse(body);
    if (json_body["mediaType"] == media_mime(MediaKind::DockerManifestListV2)) {
        return ManifestList{json_body};
    } else if (json_body["mediaType"] ==
               media_mime(MediaKind::DockerManifestV2)) {
        std::string digest = m_sess->header_field("docker-content-digest");
        return Manifest{json_body, std::move(digest)};
    }
    logs::fatal(json_body);
    raise("Unsupported mediaType: ", json_body["mediaType"]);
}

ManifestList RegistryClient::manifest_list(const Reference& ref) {
    auto res = get_resource(ref);
    if (std::holds_alternative<ManifestList>(res)) {
        return std::get<ManifestList>(res);
    }
    raise("Not a manifest-list");
}

Manifest RegistryClient::manifest(const Reference& ref) {
    auto res = get_resource(ref);
    if (std::holds_alternative<Manifest>(res)) {
        return std::get<Manifest>(res);
    }
    raise("Not a manifest");
}

std::vector<uint8_t> RegistryClient::blob(const Reference& img) {
    m_sess->set_header_field(
        "Accept",
        MediaType::from_kind(MediaKind::DockerImageRootfsDiffTarGz).mime);
    auto res = m_sess->get(blob_url(img, true));
    logs::debug("blob status code: ", res);
    auto body = m_sess->get_body();
    std::vector<uint8_t> v{body.begin(), body.end()};
    logs::debug("response size: ", v.size());
    return v;
}

// ManifestList RegistryClient::manifest_list(const Reference& img) {
//     int status_code = m_sess->get(registry_root_url(img.registry()));
//     if (status_code != 200) raise("Failure status code: ", status_code);
//     auto docker_dist_api_ver =
//         m_sess->header_field("docker-distribution-api-version");
//     logs::debug("docker api dist: '", docker_dist_api_ver, "'");
//     bool is_docker_registry = !docker_dist_api_ver.empty();
//     std::stringstream ss{};
//     get_accept(ss);
//     // m_sess->set_header_field("Accept", ss.rdbuf()->view());
//     m_sess->set_header_field(
//         "Accept",
//         util::join_inline(", ",
//         media_mime(MediaKind::DockerContainerImageV1),
//                           media_mime(MediaKind::DockerManifestV2),
//                           media_mime(MediaKind::DockerManifestListV2)));
//     logs::debug("url: ", manifest_url(img));
//     status_code = m_sess->get(manifest_url(img));
//     logs::debug("status code: ", status_code);
//     logs::trace(m_sess->get_body());
//     return ManifestList{nlohmann::json::parse(m_sess->get_body())};
// }
//
// Manifest RegistryClient::manifest(const Reference& img) {
//     std::string digest = m_sess->header_field("docker-content-digest");
//     m_sess->set_header_field(
//         "Accept", "application/vnd.docker.distribution.manifest.v2+json");
//     auto res = m_sess->get(manifest_url(img));
//     return Manifest{nlohmann::json::parse(std::string(m_sess->get_body())),
//                     std::move(digest)};
// }
//
// std::vector<uint8_t> RegistryClient::blob(const Reference& img) {
//     m_sess->set_header_field(
//         "Accept",
//         MediaType::from_kind(MediaKind::DockerImageRootfsDiffTarGz).mime);
//     auto res = m_sess->get(blob_url(img));
//     logs::debug("blob status code: ", res);
//     auto body = m_sess->get_body();
//     std::vector<uint8_t> v{body.begin(), body.end()};
//     logs::debug("response size: ", v.size());
//     return v;
// }
}  // namespace cent::oci
