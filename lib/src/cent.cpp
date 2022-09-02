#include "cent.hpp"

namespace cent {

class Cent::CentImpl {
 public:
  CentImpl(Interface* iface) : m_iface{iface} {}

 private:
  Interface* m_iface;
};

Cent::Cent(Interface* iface) : m_impl{new CentImpl(iface)} {}
Cent::~Cent() {}

}  // namespace cent