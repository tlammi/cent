#include "strg/sqlite.hpp"

#include <gtest/gtest.h>

using namespace cent::strg::sqlite;
using cent::bitmask;

TEST(Connection, Create) {
    auto conn = Connection(":memory:", bitmask() | OpenFlags::ReadWrite);
}

auto mk_conn() {
    return Connection(":memory:",
                      bitmask() | OpenFlags::ReadWrite | OpenFlags::Create);
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
