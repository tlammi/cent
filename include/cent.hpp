#pragma once

#include <memory>
#include <string_view>

#include "cent/interface.hpp"
#include "cent/log_level.hpp"
#include "cent/result.hpp"

namespace cent {

extern std::reference_wrapper<std::ostream> LOG_STREAM;

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
