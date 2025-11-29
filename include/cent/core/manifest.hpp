#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cent {

/**
 * \brief Parsed manifest
 *
 * Minimal information parsed from the message received from the server.
 * */
struct Manifest {
    // Original digest of the document received from the server
    std::string digest;
    // Digest of the config
    std::string config;
    // Digests of the layers
    std::vector<std::string> layers{};
    // Image annotations
    std::map<std::string, std::string> annotations{};
};

namespace manifest_detail {
class Impl;
}

class Manifest2 {
 public:
    explicit Manifest2(std::string v);
    explicit Manifest2(std::vector<std::byte> v);
    Manifest2(Manifest2&) = delete;
    Manifest2& operator=(Manifest2&) = delete;

    Manifest2(Manifest2&&) noexcept = default;
    Manifest2& operator=(Manifest2&&) noexcept = default;

    ~Manifest2();

    std::string_view config_digest();
    std::vector<std::string_view> layer_digests();

    std::vector<std::pair<std::string_view, std::string_view>> annotations();

    std::string_view data() noexcept;

 private:
    std::unique_ptr<manifest_detail::Impl> m_impl;
};

}  // namespace cent
