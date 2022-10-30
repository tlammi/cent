#pragma once

#include <cent/driver.hpp>
#include <filesystem>
#include <memory>
#include <vector>

namespace cent::archive {

namespace stdfs = std::filesystem;

class Driver : public ::cent::Driver {
 public:
    virtual void unpack(const stdfs::path& src, const stdfs::path& dst) = 0;
};

std::vector<std::string_view> builtin_drivers();
std::shared_ptr<Driver> builtin_driver(std::string_view name);
void set_driver(std::shared_ptr<Driver> drv);
}  // namespace cent::archive