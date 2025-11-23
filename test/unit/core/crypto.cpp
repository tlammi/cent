#include <gtest/gtest.h>

#include <cent/core/crypto.hpp>

using namespace std::literals;

TEST(Sha256, Empty) {
    auto res = cent::sha256("");
    ASSERT_EQ(
        res,
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}

TEST(Sha256, Simple) {
    static constexpr auto expected =
        "2c26b46b68ffc68ff99b453c1d30413413422d706483bfa0f98a5e886266e7ae"sv;

    auto res = cent::sha256("foo");
    ASSERT_EQ(res, expected);
}
