#include <cent/cent.hpp>

namespace cent {

class Cent::CentImpl {};

Cent::Cent(Config config) : m_impl{new Cent::CentImpl} {}
Cent::~Cent() {}

void init_defaults() {
    fs::set_driver(fs::builtin_driver("linux"));
    net::set_driver(net::builtin_driver("curl"));
}

}  // namespace cent