#include "sqlite.hpp"

namespace cent::strg::sqlite {

using namespace std::literals;

namespace {

void check(int code) {
    if (code != SQLITE_OK)
        raise(ErrorCode::Generic, "{}", sqlite3_errstr(code));
}

template <class T>
constexpr size_t flattened_size(const auto& v) {
    if constexpr (std::convertible_to<decltype(v), T>)
        return 1;
    else
        return v.size();
}

template <class T>
void flatten_apply(std::vector<T>& vec, auto&& v) {
    if constexpr (std::convertible_to<decltype(v), T>) {
        vec.push_back(std::forward<decltype(v)>(v));
    } else {
        for (auto&& value : std::forward<decltype(v)>(v)) {
            vec.push_back(std::forward<decltype(value)>(value));
        }
    }
}

template <class T, class... Ts>
std::vector<T> flatten(Ts&&... ts) {
    size_t vec_size = (flattened_size(ts) + ...);
    auto out = std::vector<T>();
    out.reserve(vec_size);
    (flatten_apply(out, std::forward<Ts>(ts)), ...);
    return out;
}

template <class... Ts>
std::string flatten_join(Ts&&... ts) {
    return flatten<std::string_view>(std::forward<Ts>(ts)...) |
           std::views::join_with(" "sv) | std::ranges::to<std::string>();
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

}  // namespace cent::strg::sqlite
