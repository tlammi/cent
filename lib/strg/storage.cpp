#include <SQLiteCpp/SQLiteCpp.h>

#include <cent/strg/storage.hpp>

namespace cent::strg {
namespace {

void simple_stmt(auto& db, const auto& str) {
    auto stmt = SQLite::Statement(db, str);
    while (stmt.executeStep());
}

auto setup_tables(SQLite::Database& db) {
    simple_stmt(db,
                "CREATE TABLE IF NOT EXISTS manifests (digest TEXT UNIQUE, "
                "data TEXT);");
    simple_stmt(db,
                "CREATE TABLE IF NOT EXISTS configs (digest TEXT UNIQUE, "
                "data TEXT);");
    simple_stmt(
        db,
        "CREATE TABLE IF NOT EXISTS images (name TEXT UNIQUE, digest TEXT);");
}

void insert_manifest(auto& db, std::string_view nm, std::string_view dt) {
    auto stmt = SQLite::Statement(db, "INSERT INTO manifests VALUES(?, ?)");
    stmt.bind(1, nm.data(), nm.size());
    stmt.bind(2, dt.data(), dt.size());
    while (stmt.executeStep());
}
}  // namespace

Storage::~Storage() = default;
class StorageImpl final : public Storage {
 public:
    explicit StorageImpl(const char* where) : m_db(where) {
        setup_tables(m_db);
    }

    void transaction_begin() override {}
    void transaction_commit() override {}
    void transaction_cancel() override {}

    void add_manifest(std::string_view digest, std::string_view data) override {
        insert_manifest(m_db, digest, data);
    }

    void add_config(std::string_view digest, std::string_view data) override {}

 private:
    SQLite::Database m_db;
};
std::unique_ptr<Storage> in_memory_storage() {
    return std::make_unique<StorageImpl>(":memory:");
}
}  // namespace cent::strg
