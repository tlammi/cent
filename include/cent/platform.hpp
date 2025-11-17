#pragma once

#include <cent/static_str.hpp>
#include <string>

namespace cent {

template <class T>
struct BasicPlatform {
    static constexpr bool is_view = std::same_as<T, std::string_view>;
    T os;
    T arch;

    constexpr operator BasicPlatform<std::string_view>() const& noexcept
        requires(!is_view)
    {
        return {.os = os, .arch = arch};
    }
};

using Platform = BasicPlatform<std::string>;
using PlatformView = BasicPlatform<std::string_view>;

#if defined(__linux__)
constexpr auto OS = StaticStr("linux");
#else
constexpr auto OS = StatiStaticStr("unknown");
#endif

#if defined(__x86_64__)
constexpr auto ARCH = StaticStr("amd64");
#else
constexpr auto ARCH = StatiStaticStr("unknown");
#endif

constexpr auto DEFAULT_PLATFORM = PlatformView{
    .os = OS.string_view(),
    .arch = ARCH.string_view(),
};

}  // namespace cent
