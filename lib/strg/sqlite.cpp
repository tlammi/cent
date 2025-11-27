#include "sqlite.hpp"

namespace cent::strg::sqlite {

using namespace std::literals;

namespace {

void check(int code) {
    if (code != SQLITE_OK)
        raise(ErrorCode::Generic, "{}", sqlite3_errstr(code));
}

auto join(std::span<std::string_view> span) {
    return span | std::views::join_with(" "sv) | std::ranges::to<std::string>();
}

}  // namespace

namespace detail {

template <>
std::int64_t column(sqlite3_stmt* stmt, int col) {
    auto res = sqlite3_column_int64(stmt, col);
    return res;
}

template <>
std::string column(sqlite3_stmt* stmt, int col) {
    const auto* data = sqlite3_column_text(stmt, col);
    return std::string(reinterpret_cast<const char*>(data));
}

template <>
std::vector<std::byte> column(sqlite3_stmt* stmt, int col) {
    const auto* data = sqlite3_column_blob(stmt, col);
    auto bytes = sqlite3_column_bytes(stmt, col);
    auto span = std::span<const std::byte>(
        reinterpret_cast<const std::byte*>(data), bytes);
    return {span.begin(), span.end()};
}

bool step_query(sqlite3_stmt* stmt) {
    auto res = sqlite3_step(stmt);
    switch (res) {
        case SQLITE_DONE: return false;
        case SQLITE_ROW: return true;
        default:
            raise(ErrorCode::Generic, "sqlite3_step(): {}",
                  sqlite3_errstr(res));
    }
}

void bind(sqlite3_stmt* stmt, int idx, int64_t val) {
    assert(idx > 0);
    auto res = sqlite3_bind_int64(stmt, idx, val);
    check(res);
}
void bind(sqlite3_stmt* stmt, int idx, std::string_view val) {
    assert(idx > 0);
    auto res = sqlite3_bind_text(stmt, idx, val.data(), val.size(), nullptr);
    check(res);
}
void bind(sqlite3_stmt* stmt, int idx, Zeros val) {
    assert(idx > 0);
    assert(val.count <= std::numeric_limits<int>::max());
    auto res = sqlite3_bind_zeroblob(stmt, idx, val.count);
    check(res);
}
void bind(sqlite3_stmt* stmt, int idx, std::span<const std::byte> val) {
    assert(idx > 0);
    assert(val.size() <= std::numeric_limits<int>::max());
    auto res = sqlite3_bind_blob(stmt, idx, val.data(), val.size(), nullptr);
    check(res);
}

}  // namespace detail

Connection::Connection(CStr str, BitMask<OpenFlags> flags) {
    using enum OpenFlags;
    int sqlite_flags = 0;
    if (flags & ReadOnly) sqlite_flags |= SQLITE_OPEN_READONLY;
    if (flags & ReadWrite) sqlite_flags |= SQLITE_OPEN_READWRITE;
    if (flags & Create) sqlite_flags |= SQLITE_OPEN_CREATE;

    auto res = sqlite3_open_v2(str.c_str(), &m_handle, sqlite_flags, nullptr);
    check(res);
}
Connection::~Connection() { sqlite3_close_v2(m_handle); }

Stmt::Stmt(Connection& c, std::string_view stmt) {
    auto res =
        sqlite3_prepare_v2(c.raw(), stmt.data(), stmt.size(), &m_s, nullptr);
    check(res);
}

Stmt::Stmt(Connection& c, std::span<std::string_view> words) {
    auto joined = join(words);
    auto res = sqlite3_prepare_v2(c.raw(), joined.data(), joined.size(), &m_s,
                                  nullptr);
    check(res);
}

Stmt::~Stmt() { sqlite3_finalize(m_s); }

void Stmt::execute() {
    // TODO: Real implementation
    while (true) {
        auto res = sqlite3_step(m_s);
        switch (res) {
            case SQLITE_DONE: return;
            case SQLITE_ROW:
                raise(ErrorCode::Generic,
                      "Received data from sqlite3 when did not expect");
                raise(ErrorCode::Generic, "{}", sqlite3_errstr(res));
        }
    }
}

namespace detail {

BlobHandle::BlobHandle(Connection& c, CStr db, CStr tbl, CStr column,
                       int64_t row, bool readwrite) {
    auto res = sqlite3_blob_open(c.raw(), db.c_str(), tbl.c_str(),
                                 column.c_str(), row, readwrite ? 1 : 0, &m_b);
    check(res);
}
BlobHandle::~BlobHandle() { sqlite3_blob_close(m_b); }
}  // namespace detail
BlobOut::BlobOut(Connection& c, CStr db, CStr tbl, CStr column, int64_t row)
    : m_b(c, db, tbl, column, row, true) {}

BlobOut& BlobOut::operator<<(std::span<const std::byte> data) {
    assert(data.size() <= std::numeric_limits<int>::max());
    auto res =
        sqlite3_blob_write(m_b.raw(), data.data(), data.size(), m_offset);
    check(res);
    m_offset += data.size();
    return *this;
}

BlobIn::BlobIn(Connection& c, CStr db, CStr tbl, CStr column, int64_t row)
    : m_b(c, db, tbl, column, row, false) {}

BlobIn& BlobIn::operator>>(std::vector<std::byte>& out) {
    assert(out.size() <= std::numeric_limits<int>::max());
    auto res = sqlite3_blob_read(m_b.raw(), out.data(), out.size(), m_offset);
    if (res == SQLITE_ERROR) {
        auto bytes = sqlite3_blob_bytes(m_b.raw());
        out.resize(bytes);
        return operator>>(out);
    }
    check(res);
    return *this;
}

}  // namespace cent::strg::sqlite
