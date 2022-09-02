#pragma once

#include <memory>
#include <string_view>

#include "cent/interface.hpp"
#include "cent/result.hpp"

namespace cent {

class Cent {
 public:
    Cent(Interface* iface);
    ~Cent();

    Result pull(std::string_view image);

 private:
    class CentImpl;
    std::unique_ptr<CentImpl> m_impl;
};
}  // namespace cent
