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

TEST(Error, Constexpr) {
    static constexpr auto e = cent::Error(E2BIG, "asdf");
    ASSERT_EQ(e.code(), cent::ErrorCode::Toobig);
    ASSERT_EQ(e.message(), "asdf");
}

TEST(Error, DynAlloc) {
    auto create = []() {
        auto msg = std::string("foo") + "bar" + "baz";
        return cent::Error(EPERM, msg);
    };
    auto e = create();
    ASSERT_EQ(e.code(), cent::ErrorCode::Perm);
    ASSERT_EQ(e.message(), "foobarbaz");
}
