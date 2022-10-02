#include "cent/runtime/config.hpp"

namespace cent::runtime {

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
    auto& user = process["user"];
    user["uid"] = 0;
    user["gid"] = 0;
}

nlohmann::json Config::json() const { return m_json; }
}  // namespace cent::runtime