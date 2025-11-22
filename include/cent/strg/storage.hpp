#pragma once

#include <filesystem>
#include <utility>

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

    virtual BlobStream* open_blob(std::string_view digest, size_t bytes) = 0;
    virtual void write_blob(BlobStream* handle, size_t blob_offset,
                            std::span<const std::byte> data) = 0;
    virtual void close_blob(BlobStream* stream) = 0;

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
        : m_s(&s), m_handle(m_s->open_blob(digest, bytes)) {}

    BlobWriteStream(const BlobWriteStream&) = delete;
    BlobWriteStream& operator=(const BlobWriteStream&) = delete;

    BlobWriteStream(BlobWriteStream&& other) noexcept
        : m_s(std::exchange(other.m_s, nullptr)), m_handle(other.m_handle) {}

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

    void write(std::span<const std::byte> data) {}

 private:
    Storage* m_s{};
    Storage::BlobStream* m_handle{};
};

std::unique_ptr<Storage> in_memory_storage();
std::unique_ptr<Storage> open_storage(const std::filesystem::path& path);
std::unique_ptr<Storage> create_storage(const std::filesystem::path& path);

}  // namespace cent::strg
