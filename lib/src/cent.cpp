#include "cent.hpp"

#include <cerrno>
#include <iostream>

#include "cent/logs.hpp"

namespace cent {

/**
 * Log consuming stream
 *
 * Set this to change where logs are written
 */
std::reference_wrapper<std::ostream> LOG_STREAM{std::cerr};

class Cent::CentImpl {
 public:
    CentImpl(Interface* iface) : m_iface{iface} {}

 private:
    Interface* m_iface;
};

Cent::Cent(Interface* iface) : m_impl{new CentImpl(iface)} {}
Cent::~Cent() {}

Result Cent::pull(std::string_view image) {
    logs::info("Not implemented yet");
    return {ENOTSUP, "Not implemented"};
}

}  // namespace cent