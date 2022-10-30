#pragma once

#include <cent/driver.hpp>
#include <cent/net/http_session.hpp>
#include <memory>
#include <string_view>
#include <vector>

namespace cent::net {

class Driver : public ::cent::Driver {
 public:
    virtual std::unique_ptr<HttpSession> http_session() = 0;

 private:
};

std::vector<std::string_view> builtin_drivers();
std::shared_ptr<Driver> builtin_driver(std::string_view name);
void set_driver(std::shared_ptr<Driver> drv);

}  // namespace cent::net