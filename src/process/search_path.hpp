#pragma once

#include <filesystem>
#include <string_view>

namespace cent::process {

std::filesystem::path search_path(std::string_view prog, std::string_view path);
std::filesystem::path search_path(std::string_view prog);
}  // namespace cent::process
