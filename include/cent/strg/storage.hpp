#pragma once

#include <cent/core/img_config.hpp>
#include <cent/data/manifest.hpp>
#include <filesystem>
#include <utility>
#include <vector>

namespace cent::strg {

class LayerBackend {
 public:
    enum class InHandle : std::uint32_t {};
    enum class OutHandle : std::uint32_t {};

    virtual OutHandle create_layer(std::string_view digest, size_t size) = 0;
    virtual InHandle open_layer(std::string_view digest) = 0;

    virtual void write(OutHandle h, std::span<const std::byte> data) = 0;
    virtual void read(InHandle h, std::vector<std::byte>& data) = 0;

    virtual void close(InHandle h) = 0;
    virtual void close(OutHandle h) = 0;

 protected:
    ~LayerBackend() = default;
};

class LayerOut {
 public:
    LayerOut(LayerBackend* backend, LayerBackend::OutHandle handle) noexcept
        : m_be(backend), m_h(handle) {}
    LayerOut(const LayerOut&) = delete;
    LayerOut& operator=(const LayerOut&) = delete;

    LayerOut(LayerOut&& other) noexcept
        : m_be(std::exchange(other.m_be, nullptr)), m_h(other.m_h) {}

    LayerOut& operator=(LayerOut&& other) noexcept {
        std::destroy_at(this);
        std::construct_at(this, std::move(other));
        return *this;
    }

    ~LayerOut() {
        if (m_be) m_be->close(m_h);
    }

    auto& operator<<(std::span<const std::byte> data) {
        m_be->write(m_h, data);
        return *this;
    }

 private:
    LayerBackend* m_be;
    LayerBackend::OutHandle m_h;
};

class LayerIn {
 public:
    LayerIn(LayerBackend* backend, LayerBackend::InHandle handle) noexcept
        : m_be(backend), m_h(handle) {}

    LayerIn(const LayerIn&) = delete;
    LayerIn& operator=(const LayerIn&) = delete;

    LayerIn(LayerIn&& other) noexcept
        : m_be(std::exchange(other.m_be, nullptr)), m_h(other.m_h) {}

    LayerIn& operator=(LayerIn&& other) noexcept {
        std::destroy_at(this);
        std::construct_at(this, std::move(other));
        return *this;
    }

    ~LayerIn() {
        if (m_be) m_be->close(m_h);
    }

    auto& operator>>(std::vector<std::byte>& data) {
        m_be->read(m_h, data);
        return *this;
    }

 private:
    LayerBackend* m_be;
    LayerBackend::InHandle m_h;
};

class Layers {
 public:
    explicit Layers(LayerBackend& backend) noexcept : m_be(&backend) {}

    LayerOut write(std::string_view digest, size_t size) {
        return {m_be, m_be->create_layer(digest, size)};
    }
    LayerIn read(std::string_view digest) {
        return {m_be, m_be->open_layer(digest)};
    }

 private:
    LayerBackend* m_be;
};

class Storage {
 public:
    virtual ~Storage() = default;

    virtual Layers layers() = 0;

    virtual void set_manifest(std::string_view digest,
                              const data::Manifest& mfest) = 0;
    virtual data::Manifest manifest(std::string_view digest) = 0;

    virtual void set_config(std::string_view digest, const ImgConfig& cfg) = 0;
    virtual ImgConfig config(std::string_view digest) = 0;
};

std::unique_ptr<Storage> in_memory_storage();
std::unique_ptr<Storage> open_storage(const std::filesystem::path& path);
std::unique_ptr<Storage> create_storage(const std::filesystem::path& path);

}  // namespace cent::strg
   // #include <cent/core/img_config.hpp>
