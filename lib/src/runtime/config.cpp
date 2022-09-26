#include "cent/runtime/config.hpp"

namespace cent::runtime {

Config::Config() {}

nlohmann::json Config::json() const { return {}; }
}  // namespace cent::runtime