#include "cent/runtime/config.hpp"

namespace cent::runtime {
namespace {

auto make_namespace(std::string_view type) {
    nlohmann::json obj{};
    obj["type"] = std::string(type);
    return obj;
}

auto make_id_mapping() {
    nlohmann::json obj{};
    obj["containerId"] = 0;
    obj["hostId"] = 10000;
    obj["size"] = 65564;
    return obj;
}

}  // namespace
Config::Config() : m_json{} {
    m_json["ociVersion"] = "1.0.1";
    auto& root = m_json["root"] = nlohmann::json::object();
    root["path"] = "rootfs";
    root["readonly"] = false;
    auto& process = m_json["process"] = nlohmann::json::object();
    process["terminal"] = true;
    process["cwd"] = "/";
    auto& env = process["env"] = nlohmann::json::array();
    env.push_back(
        "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
    auto& args = process["args"] = nlohmann::json::array();
    args.emplace_back("/bin/sh");
    auto& user = process["user"];
    user["uid"] = 0;
    user["gid"] = 0;

    auto& linux = m_json["linux"];
    auto& nspaces = linux["namespaces"] = nlohmann::json::array();
    nspaces.push_back(make_namespace("user"));

    auto& uid_mappings = linux["uidMappings"] = nlohmann::json::array();
    uid_mappings.push_back(make_id_mapping());

    auto& gid_mappings = linux["gidMappings"] = nlohmann::json::array();
    gid_mappings.push_back(make_id_mapping());
}

nlohmann::json Config::json() const { return m_json; }
}  // namespace cent::runtime