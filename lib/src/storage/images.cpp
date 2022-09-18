/* SPDX-License-Identifier:  GPL-3.0-or-later */
#include "cent/storage/images.hpp"

#include "cent/raise.hpp"

namespace cent::storage {
namespace {

/**
 * Iterate Vect until Callable returns true
 */
template <class Vect, class Callable>
bool until_entry(Vect&& v, Callable&& c) {
    for (const auto& i : v)
        if (c(i)) return true;
    return false;
}
}  // namespace

Images::Images(const nlohmann::json& json) {
    for (const auto& obj : json) {
        Digest digest{obj["digest"].get<std::string>()};
        std::vector<Reference> images{};
        images.reserve(obj["images"].size());
        for (const auto& img : obj["images"]) {
            images.emplace_back(img.get<std::string>());
        }
        Entry entry{std::move(images), std::move(digest)};
        m_entries.push_back(std::move(entry));
    }
}

auto Images::at(const Reference& image) -> Entry& {
    for (auto& e : m_entries) {
        for (auto& i : e.image_names) {
            if (i.str() == image.str()) return e;
        }
    }
    raise("No image entry for ", image);
}

auto Images::at(DigestView digest) -> Entry& {
    for (auto& e : m_entries) {
        if (e.manifest_digest.str() == digest.str()) return e;
    }
    raise("No image entry for ", digest);
}

auto Images::at(const Reference& image) const -> const Entry& {
    for (const auto& e : m_entries) {
        for (const auto& i : e.image_names) {
            if (i.str() == image.str()) return e;
        }
    }
    raise("No image entry for ", image);
}

auto Images::at(DigestView digest) const -> const Entry& {
    for (const auto& e : m_entries) {
        if (e.manifest_digest.str() == digest.str()) return e;
    }
    raise("No image entry for ", digest);
}

auto Images::operator[](Reference image) -> Entry& {
    for (auto& e : m_entries) {
        for (auto& i : e.image_names) {
            if (i.str() == image.str()) return e;
        }
    }
    m_entries.push_back(Entry{std::vector{std::move(image)}, Digest{}});
    return m_entries.back();
}

auto Images::operator[](DigestView digest) -> Entry& {
    for (auto& e : m_entries) {
        if (e.manifest_digest.str() == digest.str()) return e;
    }
    m_entries.push_back(Entry{std::vector<Reference>{}, std::move(digest)});
    return m_entries.back();
}

nlohmann::json Images::json() const {
    nlohmann::json out = nlohmann::json::array();
    for (const auto& e : m_entries) {
        nlohmann::json object = nlohmann::json::object();
        object["digest"] = e.manifest_digest.str();
        nlohmann::json images = nlohmann::json::array();
        for (const auto& i : e.image_names) { images.push_back(i.str()); }
        object["images"] = std::move(images);
        out.push_back(std::move(object));
    }
    return out;
}
auto Images::entries() const -> const std::vector<Entry>& { return m_entries; }

}  // namespace cent::storage