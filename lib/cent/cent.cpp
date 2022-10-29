#include <cent/cent.hpp>

namespace cent {

class Cent::CentImpl {};

Cent::Cent(Config config) : m_impl{new Cent::CentImpl} {}
Cent::~Cent() {}

void init_defaults() {}

}  // namespace cent