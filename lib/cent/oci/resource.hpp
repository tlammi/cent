#pragma once

#include <variant>

#include "cent/oci/manifest.hpp"
#include "cent/oci/manifest_list.hpp"

namespace cent::oci {

/**
 * Sum type for JSON resources
 */
using Resource = std::variant<ManifestList, Manifest>;

}  // namespace cent::oci