#include <gtest/gtest.h>

#include <cent/str.hpp>

using cent::str::CStr;

TEST(CStr, Ctor) {
    CStr cstr("hello");
    ASSERT_EQ(cstr, "hello");
}

TEST(CStr, CopyCtor) {
    CStr cstr("hello");
    CStr cstr2(cstr);
    ASSERT_EQ(cstr2, "hello");
}

TEST(CStr, MoveCtor) {
    CStr cstr("hello");
    CStr cstr2(std::move(cstr));
    ASSERT_EQ(cstr2, "hello");
}

