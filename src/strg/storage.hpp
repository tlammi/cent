#pragma once

#include <filesystem>
#include <ranges>
#include <utility>
#include <vector>

#include "img_config.hpp"
#include "manifest.hpp"

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

class LayerInRange {
    static constexpr size_t buffer_size = 1024 * 1024;

 public:
    explicit LayerInRange(LayerIn layer) noexcept : m_layer(std::move(layer)) {}
    class iterator {
        friend LayerInRange;

     public:
        using value_type = std::span<const std::byte>;
        using reference = value_type;
        using difference_type = std::ptrdiff_t;
        constexpr iterator() noexcept = default;

        iterator& operator++() {
            m_r->m_layer >> m_r->m_buf;
            return *this;
        }

        iterator operator++(int) {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        std::span<const std::byte> operator*() const noexcept {
            return m_r->m_buf;
        }

        bool operator==(std::default_sentinel_t) const noexcept {
            return m_r->m_buf.empty();
        }

     private:
        explicit iterator(LayerInRange* range) noexcept : m_r(range) {}
        LayerInRange* m_r{};
    };

    auto begin() noexcept { return iterator{this}; }
    constexpr auto end() const noexcept { return std::default_sentinel; }

 private:
    LayerIn m_layer;
    std::vector<std::byte> m_buf{buffer_size, std::byte{}};
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
                              std::string_view manifest) = 0;
    virtual std::string manifest(std::string_view digest) = 0;

    virtual void set_config(std::string_view digest, std::string_view cfg) = 0;
    virtual std::string config(std::string_view digest) = 0;
};

std::unique_ptr<Storage> in_memory_storage();
std::unique_ptr<Storage> open_storage(const std::filesystem::path& path);
std::unique_ptr<Storage> create_storage(const std::filesystem::path& path);

}  // namespace cent::strg
   // #include <cent/core/img_config.hpp>
