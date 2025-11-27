#include <SQLiteCpp/SQLiteCpp.h>
#include <sqlite3.h>

#include <cent/error.hpp>
#include <cent/strg/storage.hpp>
#include <cent/util/defer.hpp>

#include "sqlite.hpp"

namespace cent::strg {
namespace {
auto zero_layer(auto& db, std::string_view digest, size_t size) {
    auto [i] =
        sqlite::Stmt(db, "INSERT INTO layers VALUES(?, ?) RETURNING rowid")
            .bind(digest, sqlite::Zeros(size))
            .query_first<std::int64_t>();
    return i;
}

auto layer_idx(auto& db, std::string_view digest) {
    auto [i] =
        sqlite::Stmt(db, "SELECT rowid FROM layers WHERE digest = ? LIMIT 1")
            .bind(digest)
            .query_first<int64_t>();
    return i;
}
}  // namespace
using namespace std::literals;

class StorageImpl final : public LayerBackend, public Storage {
 public:
    explicit StorageImpl(const char* path)
        : m_db(path, bitmask() | sqlite::OpenFlags::ReadWrite |
                         sqlite::OpenFlags::Create) {
        // TODO: what if exists already?
        sqlite::execute(m_db, "CREATE TABLE layers (digest TEXT, data BLOB)");
    }

    LayerBackend::OutHandle create_layer(std::string_view digest,
                                         size_t size) override {
        auto idx = zero_layer(m_db, digest, size);
        auto unused =
            std::ranges::find_if(m_blobs_out, [](const auto& v) { return !v; });
        if (unused != m_blobs_out.end()) {
            *unused = sqlite::BlobOut(m_db, "main", "layers", "data", idx);
            uint32_t diff = unused - m_blobs_out.begin();
            return underlying_cast<LayerBackend::OutHandle>(diff);
        }
        m_blobs_out.push_back(
            sqlite::BlobOut(m_db, "main", "layers", "data", idx));
        uint32_t diff = m_blobs_out.size() - 1;
        return underlying_cast<LayerBackend::OutHandle>(diff);
    }

    LayerBackend::InHandle open_layer(std::string_view digest) override {
        auto idx = layer_idx(m_db, digest);
        auto unused =
            std::ranges::find_if(m_blobs_in, [](const auto& v) { return !v; });
        if (unused != m_blobs_in.end()) {
            *unused = sqlite::BlobIn(m_db, "main", "layers", "data", idx);
            uint32_t diff = unused - m_blobs_in.begin();
            return underlying_cast<LayerBackend::InHandle>(diff);
        }
        m_blobs_in.push_back(
            sqlite::BlobIn(m_db, "main", "layers", "data", idx));
        uint32_t diff = m_blobs_in.size() - 1;
        return underlying_cast<LayerBackend::InHandle>(diff);
    }

    void write(OutHandle h, std::span<const std::byte> data) override {
        auto offset = underlying_cast(h);
        m_blobs_out[offset] << data;
    }
    void read(InHandle h, std::vector<std::byte>& data) override {
        auto offset = underlying_cast(h);
        m_blobs_in[offset] >> data;
    }

    void close(OutHandle h) override {
        auto offset = underlying_cast(h);
        m_blobs_out[offset].clear();
    }

    void close(InHandle h) override {
        auto offset = underlying_cast(h);
        m_blobs_in[offset].clear();
    }

    Layers layers() override { return Layers{*this}; }

    void set_manifest(std::string_view digest,
                      const data::Manifest& mfest) override {}
    data::Manifest manifest(std::string_view digest) override {}

    void set_config(std::string_view digest, const ImgConfig& cfg) override {}
    ImgConfig config(std::string_view digest) override {}

 private:
    sqlite::Connection m_db;
    std::vector<sqlite::BlobOut> m_blobs_out{};
    std::vector<sqlite::BlobIn> m_blobs_in{};
};
std::unique_ptr<Storage> in_memory_storage() {
    return std::make_unique<StorageImpl>(":memory:");
}

std::unique_ptr<Storage> open_storage(const std::filesystem::path& path) {
    return std::make_unique<StorageImpl>(path.native().c_str());
}
std::unique_ptr<Storage> create_storage(const std::filesystem::path& path) {
    // TODO: Implement
    return open_storage(path);
}
}  // namespace cent::strg
