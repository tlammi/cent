#pragma once

#include <memory>
#include <span>
#include <string_view>
#include <utility>

#include "bitmask.hpp"

namespace cent::process {

enum class Namespaces {};

struct RunOptions {
    BitMask<Namespaces> namespaces{};
    bool lookup{true};
};

struct IdMap {
    size_t uid{};       //< UID inside the namespace
    size_t loweruid{};  //< UID in the parent namespace
    size_t count{};     // Number of UIDs
};

struct CloneOptions {
    BitMask<Namespaces> namespaces{};
    IdMap uidmap{};
    IdMap gidmap{};
};

int run(std::span<std::string_view> cmd, const RunOptions& opts);

int clone(const CloneOptions& opts);
}  // namespace cent::process
