#pragma once

#include <filesystem>
#include <utility>
#include <vector>

namespace cent::strg {

class Storage {
 public:
    struct BlobStream;

    virtual ~Storage();

    virtual void transaction_begin() = 0;
    virtual void transaction_commit() = 0;
    virtual void transaction_cancel() = 0;

    virtual void set_manifest(std::string_view digest,
                              std::string_view data) = 0;
    virtual bool has_manifest(std::string_view digest) = 0;
    virtual std::string manifest(std::string_view digest) = 0;

    virtual void set_config(std::string_view digest, std::string_view data) = 0;

    virtual BlobStream* open_blob_write(std::string_view digest,
                                        size_t bytes) = 0;
    virtual BlobStream* open_blob_read(std::string_view digest) = 0;
    virtual void write_blob(BlobStream* handle, size_t blob_offset,
                            std::span<const std::byte> data) = 0;

    virtual void read_blob(BlobStream* handle, size_t blob_offset,
                           std::span<std::byte> buffer) = 0;

    virtual size_t blob_size(BlobStream* handle) const = 0;

    virtual void close_blob(BlobStream* stream) = 0;

    void write_full_blob(std::string_view digest, std::string_view data) {
        write_full_blob(
            digest,
            std::span<const std::byte>(
                reinterpret_cast<const std::byte*>(data.data()), data.size()));
    }
    virtual void write_full_blob(std::string_view digest,
                                 std::span<const std::byte> blob) = 0;
    virtual std::vector<std::byte> read_full_blob(std::string_view digest) = 0;

    virtual bool has_blob(std::string_view digest) = 0;
};

class Transaction {
 public:
    constexpr Transaction() noexcept = default;
    constexpr explicit Transaction(Storage& s) : m_s(&s) {
        m_s->transaction_begin();
    }

    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;

    Transaction(Transaction&& other) noexcept;
    Transaction& operator=(Transaction&& other) noexcept;

    constexpr ~Transaction() {
        if (m_s) m_s->transaction_cancel();
    }

    void commit() {
        m_s->transaction_commit();
        m_s = nullptr;
    }

 private:
    Storage* m_s{};
};

class BlobWriteStream {
 public:
    constexpr explicit BlobWriteStream(Storage& s, std::string_view digest,
                                       size_t bytes)
        : m_s(&s), m_handle(m_s->open_blob_write(digest, bytes)) {}

    BlobWriteStream(const BlobWriteStream&) = delete;
    BlobWriteStream& operator=(const BlobWriteStream&) = delete;

    BlobWriteStream(BlobWriteStream&& other) noexcept
        : m_s(std::exchange(other.m_s, nullptr)),
          m_handle(other.m_handle),
          m_offset(other.m_offset) {}

    BlobWriteStream& operator=(BlobWriteStream&& other) noexcept {
        std::destroy_at(this);
        std::construct_at(this, std::move(other));
        return *this;
    }

    ~BlobWriteStream() {
        if (m_s) m_s->close_blob(m_handle);
    }

    void write(std::string_view data) {
        write(std::span(reinterpret_cast<const std::byte*>(data.data()),
                        data.size()));
    }

    void write(std::span<const std::byte> data) {
        m_s->write_blob(m_handle, m_offset, data);
        m_offset += data.size();
    }

 private:
    Storage* m_s{};
    Storage::BlobStream* m_handle{};
    int m_offset{};
};

class BlobReadStream {
 public:
    constexpr explicit BlobReadStream(Storage& s, std::string_view digest)
        : m_s(&s),
          m_handle(m_s->open_blob_read(digest)),
          m_tot_size(m_s->blob_size(m_handle)) {}

    ~BlobReadStream() {
        if (m_s) m_s->close_blob(m_handle);
    }

    std::vector<std::byte> read() {
        const auto buf_size = std::min<size_t>(m_tot_size - m_offset, 1024);
        std::vector<std::byte> out(buf_size);
        m_s->read_blob(m_handle, m_offset, out);
        m_offset += buf_size;
        return out;
    }

 private:
    Storage* m_s;
    Storage::BlobStream* m_handle;
    size_t m_tot_size;
    int m_offset{0};
};

std::unique_ptr<Storage> in_memory_storage();
std::unique_ptr<Storage> open_storage(const std::filesystem::path& path);
std::unique_ptr<Storage> create_storage(const std::filesystem::path& path);

}  // namespace cent::strg
