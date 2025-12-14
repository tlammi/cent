#pragma once

#include <span>
#include <string>

namespace cent {

std::string sha256(std::span<const std::byte> data);
inline std::string sha256(std::string_view s) {
    return sha256(std::span<const std::byte>(
        reinterpret_cast<const std::byte*>(s.data()), s.size()));
}

}  // namespace cent
