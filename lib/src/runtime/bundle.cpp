#include "cent/runtime/bundle.hpp"

#include "cent/logs.hpp"
#include "cent/runtime/config.hpp"

namespace cent::runtime {

Bundle::Bundle(drv::Drivers* drivers, stdfs::path dir)
    : m_drv{drivers}, m_path{std::move(dir)} {
    auto* fs = m_drv->file_system();
    logs::debug("creating bundle: ", root());
    fs->mkdir(root(), true);
    fs->mkdir(path() / "upper", true);
    fs->mkdir(path() / "work", true);
    if (!fs->exists(config_file())) {
        (*fs->open_file(config_file(), std::ios_base::out)) << Config{}.json();
    }
}

stdfs::path Bundle::root() const { return m_path / "rootfs"; }

/// Read configuration
nlohmann::json Bundle::config() { return Config{}.json(); }

/// Write config
void Bundle::config(const nlohmann::json& config) {}

/// Path to config file
stdfs::path Bundle::config_file() const { return m_path / "config.json"; }

const stdfs::path& Bundle::path() const noexcept { return m_path; }

}  // namespace cent::runtime