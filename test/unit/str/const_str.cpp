#include <gtest/gtest.h>

#include <cent/str.hpp>

using cent::str::ConstStr;
using cent::str::StaticStr;
using namespace cent::str::literals;

consteval ConstStr mk_const_str(StaticStr s) { return s; }

TEST(ConstStr, Static) {
    ConstStr cstr("hello"_static);
    ASSERT_EQ(cstr, "hello");
    ASSERT_FALSE(cstr.dynamically_allocated());
}

TEST(ConstStr, Dynamic) {
    ConstStr cstr(std::string("hello"));
    ASSERT_EQ(cstr, "hello");
    ASSERT_TRUE(cstr.dynamically_allocated());
}

TEST(ConstStr, ConstexprInit) {
    auto s = mk_const_str("hello"_static);
    ASSERT_EQ(s, "hello");
    ASSERT_FALSE(s.dynamically_allocated());
}

