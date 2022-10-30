#pragma once

#include <cent/net/driver.hpp>

namespace cent::net {

std::shared_ptr<Driver> make_curl_driver();
}