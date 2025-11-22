#include <SQLiteCpp/SQLiteCpp.h>
#include <sqlite3.h>

#include <cent/error.hpp>
#include <cent/strg/storage.hpp>
#include <cent/util/defer.hpp>

namespace cent::strg {
using namespace std::literals;
namespace {

void simple_stmt(auto& db, const auto& str) {
    auto stmt = SQLite::Statement(db, str);
    while (stmt.executeStep());
}

void create_tbl(auto& db, std::string_view tbl, std::string_view cols) {
    simple_stmt(db,
                std::format("CREATE TABLE IF NOT EXISTS {} ({});", tbl, cols));
}

auto setup_tables(SQLite::Database& db) {
    create_tbl(db, "manifests", "digest TEXT UNIQUE, data TEXT");
    create_tbl(db, "configs", "digest TEXT UNIQUE, data TEXT");
    create_tbl(db, "images", "name TEXT UNIQUE, digest TEXT");
    create_tbl(db, "blobs", "digest TEXT UNIQUE, data BLOB");
}

void insert_manifest(auto& db, std::string_view nm, std::string_view dt) {
    auto stmt =
        SQLite::Statement(db, "INSERT OR REPLACE INTO manifests VALUES(?, ?)");
    stmt.bind(1, nm.data(), nm.size());
    stmt.bind(2, dt.data(), dt.size());
    while (stmt.executeStep());
}

void insert_config(auto& db, std::string_view nm, std::string_view dt) {
    auto stmt =
        SQLite::Statement(db, "INSERT OR REPLACE INTO configs VALUES(?, ?)");
    stmt.bind(1, nm.data(), nm.size());
    stmt.bind(2, dt.data(), dt.size());
    while (stmt.executeStep());
}

bool contains(auto& db, std::string_view tbl, std::string_view col,
              std::string_view val) {
    auto stmt = SQLite::Statement(
        db,
        std::format("SELECT EXISTS (SELECT 1 FROM {} WHERE {} = ?)", tbl, col));
    stmt.bind(1, val.data(), val.size());
    assert(stmt.executeStep());
    return stmt.getColumn(0).getInt() != 0;
}

}  // namespace

Storage::~Storage() = default;
class StorageImpl final : public Storage {
 public:
    explicit StorageImpl(const char* where)
        : m_db(where, SQLite::OPEN_READWRITE) {
        setup_tables(m_db);
    }

    void transaction_begin() override { m_db.exec("BEGIN TRANSACTION"); }
    void transaction_commit() override { m_db.exec("COMMIT TRANSACTION"); }
    void transaction_cancel() override { m_db.exec("ROLLBACK TRANSACTION"); }

    void set_manifest(std::string_view digest, std::string_view data) override {
        insert_manifest(m_db, digest, data);
    }

    bool has_manifest(std::string_view digest) override {
        return contains(m_db, "manifests", "digest", digest);
    }

    std::string manifest(std::string_view digest) override {
        auto stmt = SQLite::Statement(
            m_db, "SELECT data from manifests WHERE digest = ?;");
        stmt.bind(1, digest.data(), digest.size());
        auto has_data = stmt.executeStep();
        if (!has_data)
            raise(ErrorCode::DoesNotExist, "Storage::manifest({})", digest);
        return std::string(stmt.getColumn(0));
    }

    void set_config(std::string_view digest, std::string_view data) override {
        insert_config(m_db, digest, data);
    }

    BlobStream* open_blob(std::string_view digest, size_t bytes) override {
        sqlite3_stmt* stmt{};
        static constexpr auto query =
            "INSERT OR REPLACE INTO blobs VALUES(?, ?);"sv;
        auto res = sqlite3_prepare_v2(m_db.getHandle(), query.data(),
                                      query.size(), &stmt, nullptr);
        if (res != SQLITE_OK)
            raise(ErrorCode::Generic, "{}", sqlite3_errstr(res));
        // Blob here since sqlitecpp uses that under the hood :(
        sqlite3_bind_blob(stmt, 1, digest.data(), digest.size(), nullptr);
        sqlite3_bind_zeroblob(stmt, 2, bytes);
        while (sqlite3_step(stmt) != SQLITE_DONE);
        sqlite3_finalize(stmt);
        auto rowid_stmt = SQLite::Statement(
            m_db, "SELECT rowid FROM blobs WHERE (digest = ?);");
        rowid_stmt.bind(1, digest.data(), digest.size());
        assert(rowid_stmt.executeStep());
        auto rowid = rowid_stmt.getColumn(0).getUInt();
        assert(!rowid_stmt.executeStep());
        sqlite3_blob* handle = nullptr;
        res = sqlite3_blob_open(m_db.getHandle(), "main", "blobs", "data",
                                rowid, 0, &handle);
        if (res != SQLITE_OK)
            raise(ErrorCode::Generic, "{}", sqlite3_errstr(res));
        return reinterpret_cast<BlobStream*>(handle);
    }

    void write_blob(BlobStream* handle, size_t blob_offset,
                    std::span<const std::byte> data) override {}

    void close_blob(BlobStream* stream) override {
        auto* handle = reinterpret_cast<sqlite3_blob*>(stream);
        sqlite3_blob_close(handle);
    }

    bool has_blob(std::string_view digest) override {
        return contains(m_db, "blobs", "digest", digest);
    }

 private:
    SQLite::Database m_db;
};
std::unique_ptr<Storage> in_memory_storage() {
    return std::make_unique<StorageImpl>(":memory:");
}
}  // namespace cent::strg
