#pragma once

#include <nlohmann/json.hpp>
#include <ostream>
#include <vector>

#include "cent/digest.hpp"
#include "cent/media_type.hpp"

namespace cent {
class Manifest {
 public:
    struct Config {
        MediaType media_type;
        size_t size;
        Digest digest;
    };
    struct Layer {
        MediaType media_type;
        size_t size;
        Digest digest;
    };
    Manifest();
    explicit Manifest(const nlohmann::json& json);

    MediaType media_type() const noexcept;
    size_t schema_version() const noexcept;
    const Config& config() const noexcept;
    const std::vector<Layer>& layers() const noexcept;

 private:
    MediaType m_media_type{};
    size_t m_schema_version{};
    Config m_config{};
    std::vector<Layer> m_layers{};
};

std::ostream& operator<<(std::ostream& os, const Manifest& manifest);
}  // namespace cent