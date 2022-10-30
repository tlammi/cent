#pragma once

#include <nlohmann/json.hpp>

namespace cent::oci::runtime {

class Config {
 public:
    Config();

    nlohmann::json json() const;

 private:
    nlohmann::json m_json;
};
}  // namespace cent::oci::runtime