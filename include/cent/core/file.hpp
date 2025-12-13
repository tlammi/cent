#pragma once

#include <cent/core/bitmask.hpp>
#include <filesystem>
#include <memory>
#include <span>

namespace cent {
enum class SeekOrigin : std::uint8_t {
    Set,
    Current,
    End,
};

class FileOut {
 public:
    virtual ~FileOut() = default;
    virtual std::span<std::byte> buffer() = 0;
    virtual void write() = 0;
    virtual void seek(long offset, SeekOrigin orig) = 0;
};

class AnyFileOut final : public FileOut {
 public:
    explicit AnyFileOut(std::unique_ptr<FileOut> impl) noexcept
        : m_impl(std::move(impl)) {}

    std::span<std::byte> buffer() override { return m_impl->buffer(); }
    void write() override { m_impl->write(); }

    void seek(long offset, SeekOrigin orig) override {
        m_impl->seek(offset, orig);
    }

 private:
    std::unique_ptr<FileOut> m_impl;
};

AnyFileOut write_memory_mapped(const std::filesystem::path& path, size_t size);

}  // namespace cent
