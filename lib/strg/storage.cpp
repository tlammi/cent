#include <SQLiteCpp/SQLiteCpp.h>
#include <sqlite3.h>

#include <cent/error.hpp>
#include <cent/strg/storage.hpp>
#include <cent/util/defer.hpp>

#include "sqlite.hpp"

namespace cent::strg {
namespace {
auto zero_layer(auto& db, size_t size) {
    for (auto [i] :
         sqlite::Stmt(db, "INSERT INTO layers VALUES(?) RETURNING rowid")
             .bind(sqlite::Zeros(size))
             .query<std::int64_t>()) {
        assert(i >= 0);
        return size_t(i);
    }
    assert(false);
}
}  // namespace
using namespace std::literals;

class StorageImpl final : public LayerBackend, public Storage {
 public:
    explicit StorageImpl(const char* path)
        : m_db(path, bitmask() | sqlite::OpenFlags::ReadWrite |
                         sqlite::OpenFlags::Create) {}

    LayerBackend::Handle create_layer(std::string_view digest,
                                      size_t size) override {
        auto idx = zero_layer(m_db, size);
        auto unused =
            std::ranges::find_if(m_blobs_out, [](const auto& v) { return !v; });
        if (unused != m_blobs_out.end()) {
            *unused = sqlite::BlobOut(m_db, "main", "layers", "data", idx);
            uint32_t diff = unused - m_blobs_out.begin();
            return static_cast<LayerBackend::Handle>(diff);
        }
        m_blobs_out.push_back(
            sqlite::BlobOut(m_db, "main", "layers", "data", idx));
        uint32_t diff = m_blobs_out.size() - 1;
        return static_cast<LayerBackend::Handle>(diff);
    }

    LayerBackend::Handle open_layer(std::string_view digest) override {}

    void write(Handle h, std::span<const std::byte> data) override {}
    void read(Handle h, std::vector<std::byte>& data) override {}

    void close(Handle h) override {}

    Layers layers() override {}

    void set_manifest(std::string_view digest,
                      const data::Manifest& mfest) override {}
    data::Manifest manifest(std::string_view digest) override {}

    void set_config(std::string_view digest, const ImgConfig& cfg) override {}
    ImgConfig config(std::string_view digest) override {}

 private:
    sqlite::Connection m_db;
    std::vector<sqlite::BlobOut> m_blobs_out{};
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
