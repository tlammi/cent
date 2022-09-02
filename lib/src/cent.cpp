#include "cent.hpp"

#include <cerrno>

namespace cent {

class Cent::CentImpl {
 public:
    CentImpl(Interface* iface) : m_iface{iface} {}

 private:
    Interface* m_iface;
};

Cent::Cent(Interface* iface) : m_impl{new CentImpl(iface)} {}
Cent::~Cent() {}

Result Cent::pull(std::string_view image) {
    return {ENOTSUP, "Not implemented"};
}

}  // namespace cent