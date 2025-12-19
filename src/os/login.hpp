#pragma once

#include <string>

namespace cent::os {

std::string getlogin(uid_t uid);
std::string getlogin();
uid_t getuid(const char* name);

}  // namespace cent::os
