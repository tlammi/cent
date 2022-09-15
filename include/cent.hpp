#pragma once

#include <memory>
#include <string_view>

#include "cent/drv/drivers.hpp"
#include "cent/log_level.hpp"
#include "cent/result.hpp"

namespace cent {

extern std::reference_wrapper<std::ostream> LOG_STREAM;
extern std::reference_wrapper<std::ostream> OUT_STREAM;

class Cent {
 public:
    Cent(drv::Drivers* drivers);
    ~Cent();

    Result pull(std::string_view image);
    Result image_list();

 private:
    class CentImpl;
    std::unique_ptr<CentImpl> m_impl;
};
}  // namespace cent
