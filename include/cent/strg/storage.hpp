#pragma once

#include <filesystem>
#include <memory>

namespace cent::strg {

class Storage {
 public:
    virtual ~Storage();

    virtual void transaction_begin() = 0;
    virtual void transaction_commit() = 0;
    virtual void transaction_cancel() = 0;

    virtual void set_manifest(std::string_view digest,
                              std::string_view data) = 0;
    virtual bool has_manifest(std::string_view digest) = 0;
    virtual std::string manifest(std::string_view digest) = 0;
    virtual void add_config(std::string_view digest, std::string_view data) = 0;
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

std::unique_ptr<Storage> in_memory_storage();
std::unique_ptr<Storage> open_storage(const std::filesystem::path& path);
std::unique_ptr<Storage> create_storage(const std::filesystem::path& path);

}  // namespace cent::strg
