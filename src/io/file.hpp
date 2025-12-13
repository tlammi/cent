#pragma once

#include <filesystem>
#include <memory>
#include <span>
#include <utility>

namespace cent::io {
enum class SeekOrigin : std::uint8_t {
    Set,
    Current,
    End,
};

struct openr_t {};
constexpr openr_t openr{};

struct openw_t {};
constexpr openw_t openw{};

struct openrw_t {};
constexpr openrw_t openrw{};

consteval openrw_t operator|(openr_t, openw_t) noexcept { return {}; }
consteval openrw_t operator|(openw_t, openr_t) noexcept { return {}; }

class File {
 public:
    constexpr File() noexcept = default;
    explicit File(FILE* stream) noexcept : m_str(stream) {}
    File(const File&) = delete;
    File& operator=(const File&) = delete;

    File(File&& other) noexcept : m_str(std::exchange(other.m_str, nullptr)) {}

    File& operator=(File&& other) noexcept {
        std::destroy_at(this);
        std::construct_at(this, std::move(other));
        return *this;
    }

    ~File();

    FILE* handle() noexcept;
    int fd();
    void seek(int64_t offset, SeekOrigin orig);
    int64_t position() const;
    int64_t size();

 private:
    FILE* m_str{};
};

class FileI : virtual public File {
 public:
    using File::File;
    size_t read(std::span<std::byte> buf);
    size_t read(std::span<char> buf) {
        return read(std::span<std::byte>(
            reinterpret_cast<std::byte*>(buf.data()), buf.size()));
    }

    template <class T>
    auto& operator>>(T& t) {
        auto pos = position();
        auto sz = size();
        t.resize(sz - pos);
        read(t);
        return *this;
    }
};

class FileO : virtual public File {
 public:
    using File::File;

    void write(std::span<const std::byte> buf);
    void write(std::string_view buf) {
        return write(std::span<const std::byte>(
            reinterpret_cast<const std::byte*>(buf.data()), buf.size()));
    }
};

class FileIO : public FileI, public FileO {
 public:
    using FileI::FileI;
};

FileI open(const std::filesystem::path& path, openr_t);
FileO open(const std::filesystem::path& path, openw_t);
FileIO open(const std::filesystem::path& path, openrw_t);

FileIO open_mem(std::span<std::byte> buf);

FileIO tmpfile();

inline FileIO open_mem(std::span<char> buf) {
    return open_mem(
        std::span(reinterpret_cast<std::byte*>(buf.data()), buf.size()));
}

class MemMappedI {
 public:
    explicit MemMappedI(const File& f);
    MemMappedI(const MemMappedI&) = delete;
    MemMappedI& operator=(const MemMappedI&) = delete;

    MemMappedI(MemMappedI&& other) noexcept
        : m_ptr(std::exchange(other.m_ptr, nullptr)), m_len(other.m_len) {}

    MemMappedI& operator=(MemMappedI&& other) noexcept {
        std::destroy_at(this);
        std::construct_at(this, std::move(other));
        return *this;
    }

    ~MemMappedI();

    std::span<const std::byte> buffer();

 private:
    void* m_ptr;
    size_t m_len;
};

class MemMappedO {
 public:
    explicit MemMappedO(const File& f);
    MemMappedO(const MemMappedO&) = delete;
    MemMappedO& operator=(const MemMappedO&) = delete;

    MemMappedO(MemMappedO&& other) noexcept
        : m_ptr(std::exchange(other.m_ptr, nullptr)), m_len(other.m_len) {}

    MemMappedO& operator=(MemMappedO&& other) noexcept {
        std::destroy_at(this);
        std::construct_at(this, std::move(other));
        return *this;
    }

    ~MemMappedO();

    std::span<std::byte> buffer();

 private:
    void* m_ptr;
    size_t m_len;
};

}  // namespace cent::io
