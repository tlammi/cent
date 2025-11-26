#include "strg/sqlite.hpp"

#include <gtest/gtest.h>

using namespace cent::strg::sqlite;
using cent::bitmask;
using namespace std::literals;

TEST(Connection, Create) {
    auto conn = Connection(":memory:", bitmask() | OpenFlags::ReadWrite);
}

auto mk_conn() {
    return Connection(":memory:",
                      bitmask() | OpenFlags::ReadWrite | OpenFlags::Create);
}

template <class... Ts>
auto mk_vec_recurse(auto& vec, uint8_t byte, Ts&&... ts) {
    vec.push_back(std::byte{byte});
    if constexpr (sizeof...(Ts)) { mk_vec_recurse(vec, ts...); }
}
auto mk_vec(auto&&... args) {
    auto out = std::vector<std::byte>();
    out.reserve(sizeof...(args));
    mk_vec_recurse(out, args...);
    return out;
}

TEST(Stmt, Read) {
    auto conn = mk_conn();
    auto res = Stmt(conn, "pragma user_version;").query<int64_t>() |
               std::ranges::to<std::vector>();
    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(std::get<0>(res[0]), 0);
}

TEST(Stmt, Write) {
    auto conn = mk_conn();
    Stmt(conn, "PRAGMA user_version = 69;").execute();
    auto res = Stmt(conn, "pragma user_version;").query<int64_t>() |
               std::ranges::to<std::vector>();
    ASSERT_EQ(std::get<0>(res[0]), 69);
}

TEST(Stmt, Bind) {
    auto conn = mk_conn();
    Stmt(conn, "CREATE TABLE tbl (id INTEGER PRIMARY KEY, text TEXT)")
        .execute();
    Stmt(conn, "INSERT INTO tbl VALUES(?, ?)").bind(1, "asdf").execute();
    auto res = Stmt(conn, "SELECT * FROM tbl;").query<int64_t, std::string>() |
               std::ranges::to<std::vector>();
    ASSERT_EQ(res.size(), 1);
    auto [i, s] = res.at(0);
    ASSERT_EQ(i, 1);
    ASSERT_EQ(s, "asdf");
}

TEST(Blob, Write) {
    auto conn = mk_conn();
    execute(conn, "CREATE TABLE tbl (data BLOB)");
    Stmt(conn, "INSERT INTO tbl VALUES (?)").bind(Zeros(1024)).execute();
    {
        auto stream = BlobOut(conn, "main", "tbl", "data", 1);
        auto data = mk_vec(0x01, 0x02, 0x03);
        stream << data;
    }
    auto res = Stmt(conn, "SELECT * FROM tbl").query<std::vector<std::byte>>() |
               std::ranges::to<std::vector>();
    ASSERT_EQ(res.size(), 1);
    auto data = std::move(std::get<0>(res[0]));
    ASSERT_EQ(data.size(), 1024);
    ASSERT_EQ(data[0], std::byte{0x01});
    ASSERT_EQ(data[1], std::byte{0x02});
    ASSERT_EQ(data[2], std::byte{0x03});
    for (size_t i = 3; i < data.size(); ++i) {
        ASSERT_EQ(data[i], std::byte{0});
    }
}

TEST(Blob, Read) {
    auto conn = mk_conn();
    execute(conn, "CREATE TABLE tbl (data BLOB)");
    auto data = mk_vec(0x01, 0x02, 0x03, 0x04);
    Stmt(conn, "INSERT INTO tbl VALUES(?)").bind(data).execute();
}
