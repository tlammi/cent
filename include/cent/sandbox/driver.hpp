#pragma once

#include <cent/driver.hpp>
#include <cent/sandbox/sandbox.hpp>
#include <functional>
#include <memory>
#include <string_view>
#include <vector>

namespace cent::sandbox {

class Driver : public ::cent::Driver {
 public:
    virtual std::unique_ptr<Sandbox> sandbox() = 0;

 private:
};

std::vector<std::string_view> builtin_drivers();
std::shared_ptr<Driver> builtin_driver(std::string_view name);
void set_driver(std::shared_ptr<Driver> drv);
}  // namespace cent::sandbox