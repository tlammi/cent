#include "cent/runtime/bundle.hpp"

namespace cent::runtime {

Bundle::Bundle(drv::Drivers* drivers, stdfs::path dir)
    : m_path{std::move(dir)} {}

stdfs::path Bundle::root() const { return m_path / "root"; }

/// Read configuration
nlohmann::json Bundle::config() { return {}; }

/// Write config
void Bundle::config(const nlohmann::json& config) {}

/// Path to config file
stdfs::path Bundle::config_file() const { return m_path / "config.json"; }

const stdfs::path& Bundle::path() const noexcept { return m_path; }

}  // namespace cent::runtime