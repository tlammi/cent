#pragma once

#include "io/file.hpp"

namespace cent::io {

class MemMapRO {
 public:
    constexpr MemMapRO() noexcept = default;
    explicit MemMapRO(File& f);
    MemMapRO(const MemMapRO&) = delete;
    MemMapRO& operator=(const MemMapRO&) = delete;

    MemMapRO(MemMapRO&& other) noexcept
        : m_span(std::exchange(other.m_span, {})) {}
    MemMapRO& operator=(MemMapRO&& other) noexcept {
        std::destroy_at(this);
        std::construct_at(this, std::move(other));
        return *this;
    }
    ~MemMapRO();

    const std::byte* data() const noexcept { return m_span.data(); }
    size_t size() const noexcept { return m_span.size(); }

    operator std::span<const std::byte>() const& noexcept { return m_span; }

    auto begin() const noexcept { return m_span.begin(); }
    auto end() const noexcept { return m_span.end(); }

 private:
    std::span<const std::byte> m_span;
};

class MemMapRW {
 public:
    constexpr MemMapRW() noexcept = default;
    explicit MemMapRW(File& f);
    MemMapRW(const MemMapRW&) = delete;
    MemMapRW& operator=(const MemMapRW&) = delete;

    MemMapRW(MemMapRW&& other) noexcept
        : m_span(std::exchange(other.m_span, {})) {}

    MemMapRW& operator=(MemMapRW&& other) noexcept {
        std::destroy_at(this);
        std::construct_at(this, std::move(other));
        return *this;
    }

    ~MemMapRW();

    std::byte* data() const noexcept { return m_span.data(); }
    size_t size() const noexcept { return m_span.size(); }

    operator std::span<std::byte>() const& noexcept { return m_span; }

    auto begin() const noexcept { return m_span.begin(); }
    auto end() const noexcept { return m_span.end(); }

 private:
    std::span<std::byte> m_span;
};
}  // namespace cent::io
