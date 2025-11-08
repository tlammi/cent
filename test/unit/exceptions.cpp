#include <gtest/gtest.h>

#include <cent/exceptions.hpp>
#include <print>

TEST(Exceptions, DefaultThrow) {
    try {
        throw cent::PermissionDenied();
    } catch (const cent::PermissionDenied& e) {
        ASSERT_NE(e.what(), std::string_view());
        return;
    }
    FAIL();
}

TEST(Exceptions, StrThrow) {
    try {
        throw cent::PermissionDenied("asdf");
    } catch (const cent::PermissionDenied& e) {
        ASSERT_EQ(e.what(), std::string_view("asdf"));
        return;
    }
    FAIL();
}

TEST(Exceptions, FmtThrow) {
    try {
        cent::raise<cent::PermissionDenied>("{},{}", 1, 2);
    } catch (const cent::PermissionDenied& e) {
        ASSERT_EQ(e.what(), std::string_view("1,2"));
        return;
    }
    FAIL();
}
