#pragma once

#include <memory>

#include "cent/interface.hpp"

namespace cent {

class Cent {
 public:
  Cent(Interface* iface);
  ~Cent();

 private:
  class CentImpl;
  std::unique_ptr<CentImpl> m_impl;
};
}  // namespace cent
