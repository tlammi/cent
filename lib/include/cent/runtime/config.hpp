#pragma once

#include <nlohmann/json.hpp>

namespace cent::runtime {

class Config {
 public:
    Config();

    nlohmann::json json() const;

 private:
};
}  // namespace cent::runtime