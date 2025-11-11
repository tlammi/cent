#pragma once

#include <cent/util/enum_arr.hpp>
#include <rfl.hpp>

namespace cent::data {

enum class Mime {
    OciImageIndex,
    OciImageManifest,
};

}  // namespace cent::data
template <>
class rfl::Reflector<cent::data::Mime> {
    using ReflType = std::string;
    static rfl::Result<cent::data::Mime> to(const std::string& str) noexcept {}
};
