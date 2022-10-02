#include "cent/runtime/bundle.hpp"

#include "cent/drv/file_system_impl.hpp"
#include "cent/logs.hpp"
#include "cent/runtime/config.hpp"

namespace cent::runtime {

Bundle::Bundle(drv::Drivers* drivers, stdfs::path dir)
    : m_drv{drivers}, m_path{std::move(dir)} {
    logs::debug("creating bundle: ", root());
    drv::fs().mkdir(root(), true);
    drv::fs().mkdir(path() / "upper", true);
    drv::fs().mkdir(path() / "work", true);
    if (!drv::fs().exists(config_file())) {
        (*drv::fs().open_file(config_file(), std::ios_base::out))
            << Config{}.json();
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