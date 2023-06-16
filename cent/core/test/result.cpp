#include <gtest/gtest.h>

#include <cent/core.hpp>

TEST(Init, Null) {
    cent::NullResult res{};
    ASSERT_FALSE(res);
}

TEST(Init, UnitResult) {
    cent::UnitResult res{};
    ASSERT_TRUE(res);
}

TEST(Init, Normal) {
    cent::Result<int> res{};
    ASSERT_TRUE(res);
    ASSERT_EQ(*res, 0);
}

TEST(Init, UnitFromNull) {
    cent::NullResult null{};
    cent::UnitResult unit{null};
    ASSERT_FALSE(unit);
}

TEST(Init, NormalFromNull) {
    cent::NullResult null{};
    cent::Result<int> res{null};
    ASSERT_FALSE(res);
}

TEST(Translate, Simple) {
    auto val = cent::make_value<int>(100);
    cent::Result<char> val2 = val.translate([](int i) -> char { return i; });
    static_assert(std::is_same_v<decltype(val2), cent::Result<char>>);
}
