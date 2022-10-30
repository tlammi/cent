/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */

#include "registry_client.hpp"

#include <array>
#include <sstream>

#include "cent/logs.hpp"
#include "cent/raise.hpp"

namespace cent::oci {

namespace {

template <class... Ts>
consteval std::array<std::string_view, sizeof...(Ts)> make_array(Ts&&... ts) {
    return {std::forward<Ts>(ts)...};
}

constexpr auto DOCKER_ACCEPT =
    make_array("application/vnd.docker.distribution.manifest.list.v2+json");

constexpr auto OCI_ACCEPT = make_array("manifest.list.v2+json");

enum class AcceptIndex {
    ManifestList,
};

std::string_view get_accept(AcceptIndex index, bool is_docker) {
    if (is_docker) return DOCKER_ACCEPT.at(static_cast<size_t>(index));
    return OCI_ACCEPT.at(static_cast<size_t>(index));
}

std::string registry_root_url(std::string_view registry) {
    std::stringstream ss{};
    ss << "https://";
    if (registry == "docker.io") ss << "registry-1.";
    ss << registry << "/v2/";
    return ss.str();
}

std::string manifest_url(const Reference& img) {
    auto registry = img.registry();
    auto name = img.name();
    auto ref = img.digest().str();
    if (ref == "") ref = img.tag();
    std::stringstream url{};
    url << registry_root_url(registry);
    url << name << "/manifests/" << ref;
    return url.str();
}

std::string blob_url(const Reference& img) {
    auto registry = img.registry();
    auto name = img.name();
    auto ref = img.digest().str();
    if (ref == "") ref = img.tag();
    std::stringstream url{};
    url << registry_root_url(registry);
    url << name << "/blobs/" << ref;
    return url.str();
}
}  // namespace
RegistryClient::RegistryClient(net::HttpSession* sess) : m_sess{sess} {
    m_sess->capture_header_field("docker-distribution-api-version");
    m_sess->capture_header_field("docker-content-digest");
}

ManifestList RegistryClient::manifest_list(const Reference& img) {
    int status_code = m_sess->get(registry_root_url(img.registry()));
    if (status_code != 200) raise("Failure status code: ", status_code);
    auto docker_dist_api_ver =
        m_sess->header_field("docker-distribution-api-version");
    logs::debug("docker api dist: '", docker_dist_api_ver, "'");
    bool is_docker_registry = !docker_dist_api_ver.empty();
    std::stringstream ss{};
    ss << get_accept(AcceptIndex::ManifestList, is_docker_registry);
    m_sess->set_header_field("Accept", ss.rdbuf()->view());
    logs::debug("url: ", manifest_url(img));
    status_code = m_sess->get(manifest_url(img));
    logs::debug("status code: ", status_code);
    return ManifestList{nlohmann::json::parse(m_sess->get_body())};
}

Manifest RegistryClient::manifest(const Reference& img) {
    std::string digest = m_sess->header_field("docker-content-digest");
    m_sess->set_header_field(
        "Accept", "application/vnd.docker.distribution.manifest.v2+json");
    auto res = m_sess->get(manifest_url(img));
    return Manifest{nlohmann::json::parse(std::string(m_sess->get_body())),
                    std::move(digest)};
}

std::vector<uint8_t> RegistryClient::blob(const Reference& img) {
    m_sess->set_header_field(
        "Accept",
        MediaType::from_kind(MediaKind::DockerImageRootfsDiffTarGz).mime);
    auto res = m_sess->get(blob_url(img));
    logs::debug("blob status code: ", res);
    auto body = m_sess->get_body();
    std::vector<uint8_t> v{body.begin(), body.end()};
    logs::debug("response size: ", v.size());
    return v;
}
}  // namespace cent