#pragma once

#include <cent/util/cast.hpp>
#include <compare>

namespace cent::logs {

enum class Level {
    Trace,  // debug WHERE execution is
    Debug,  // debug WHAT program is doing
    Info,   // more generic information
    Note,   // more important than info but not a warning
    Warn,   // abnormal but can be managed
    Err,    // abnormal and cannot be managed, but does not cause a crash
    Fatal,  // abnormal causing a crash
    COUNT_,
};

constexpr bool operator==(Level l, Level r) {
    return underlying_cast(l) == underlying_cast(r);
}

constexpr bool operator!=(Level l, Level r) { return !(l == r); }

constexpr auto operator<=>(Level l, Level r) {
    return underlying_cast(l) <=> underlying_cast(r);
}

}  // namespace cent::logs
