/* SPDX-License-Identifier:  GPL-3.0-or-later */
#pragma once

#include <nlohmann/json.hpp>
#include <vector>

#include "cent/digest.hpp"
#include "cent/reference.hpp"

namespace cent::storage {

/**
 * Access storage's image database
 */
class Images {
 public:
    struct Entry {
        std::vector<Reference> image_names;
        Digest manifest_digest;
    };

    Images(const nlohmann::json& json);

    Entry& at(const Reference& image);
    Entry& at(DigestView digest);
    const Entry& at(const Reference& image) const;
    const Entry& at(DigestView digest) const;

    Entry& operator[](Reference image);
    Entry& operator[](DigestView image);

    const std::vector<Entry>& entries() const;

    nlohmann::json json() const;

 private:
    std::vector<Entry> m_entries{};
};
}  // namespace cent::storage