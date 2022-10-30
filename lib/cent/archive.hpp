#pragma once

#include <filesystem>
namespace cent::archive {

namespace stdfs = std::filesystem;

void unpack(const stdfs::path& src, const stdfs::path& dst);
}