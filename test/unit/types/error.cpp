#include <gtest/gtest.h>

#include <cent/types.hpp>

TEST(Error, FromErrno) {
    auto e = cent::Error(EPERM);
    ASSERT_EQ(e.code(), cent::ErrorCode::Perm);
}

TEST(Error, CustomMsg) {
    auto e = cent::Error(EPERM, "Custom");
    ASSERT_EQ(e.code(), cent::ErrorCode::Perm);
    ASSERT_EQ(e.message(), "Custom");
}
