#include "bundle.hpp"

#include "cent/fs.hpp"
#include "cent/logs.hpp"
#include "cent/oci/runtime/config.hpp"
#include "cent/raise.hpp"

namespace cent::oci::runtime {

Bundle::Bundle(stdfs::path dir, const std::vector<stdfs::path>& layers)
    : m_path{std::move(dir)} {
    logs::debug("creating bundle: ", root());
    fs::mkdir(root(), true);
    fs::mkdir(path() / "upper", true);
    fs::mkdir(path() / "work", true);
    if (!fs::exists(config_file())) {
        (*fs::open_file(config_file(), std::ios_base::out)) << Config{}.json();
    }
    auto fs = fs::open_file(path() / "layers", std::ios_base::out);
    for (const auto& l : layers) { (*fs) << l.string() << '\n'; }
}

Bundle::Bundle(stdfs::path dir) : m_path{std::move(dir)} {
    if (!fs::exists(m_path)) raise("Bundle path ", m_path, " does not exist");
}

stdfs::path Bundle::root() const { return m_path / "rootfs"; }

/// Read configuration
nlohmann::json Bundle::config() { return Config{}.json(); }

/// Write config
void Bundle::config(const nlohmann::json& config) {}

/// Path to config file
stdfs::path Bundle::config_file() const { return m_path / "config.json"; }

const stdfs::path& Bundle::path() const noexcept { return m_path; }

}  // namespace cent::oci::runtime