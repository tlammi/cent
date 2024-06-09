#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cent/cli/parser.hpp>

using cent::cli::Branch;
using cent::cli::BranchBuilder;
using cent::cli::Leaf;
using cent::cli::LeafBuilder;

template <class... Ts>
auto mk_args(Ts&&... ts) {
    // NOLINTNEXTLINE(*-pointer-decay)
    return std::vector<const char*>{std::forward<Ts>(ts)...};
}

TEST(Leaf, NoArgs) {
    auto leaf = LeafBuilder().name("foo").description("bar").commit();
    ASSERT_TRUE(leaf.parse(0, nullptr));
}

TEST(Leaf, PosArgStr) {
    std::string_view s{};
    LeafBuilder().pos_arg("s", &s, "string").commit().parse(mk_args("foo"));
    ASSERT_EQ(s, "foo");
}

TEST(Leaf, PosArgBool) {
    bool a{}, b{}, c{}, d{}, e{};
    LeafBuilder()
        .pos_arg("a", &a, "a")
        .pos_arg("b", &b, "b")
        .pos_arg("c", &c, "c")
        .pos_arg("d", &d, "d")
        .pos_arg("e", &e, "e")
        .commit()
        .parse(mk_args("true", "false", "1", "0", "TRUE"));
    ASSERT_TRUE(a);
    ASSERT_FALSE(b);
    ASSERT_TRUE(c);
    ASSERT_FALSE(d);
    ASSERT_TRUE(e);
}

TEST(Leaf, PosArgInt) {
    int i = 0;
    auto leaf = LeafBuilder()
                    .name("foo")
                    .description("bar")
                    .pos_arg("i", &i, "integer")
                    .commit();

    auto args = mk_args("42");
    ASSERT_TRUE(leaf.parse(args));
    ASSERT_EQ(i, 42);
}

TEST(Leaf, PosArgVecInt) {
    std::vector<int> is{};
    auto leaf = LeafBuilder().pos_arg("i", &is, "integers").commit();
    auto args = mk_args("42", "43", "44");
    ASSERT_TRUE(leaf.parse(args));
    ASSERT_THAT(is, testing::ElementsAre(42, 43, 44));  // NOLINT
}

TEST(Leaf, OptShort) {
    int i{};
    auto leaf = LeafBuilder().opt('i', "i", &i, "integer").commit();
    auto res = leaf.parse(mk_args("-i", "1"));
    ASSERT_TRUE(res) << res.error().message();
    ASSERT_EQ(i, 1);
}

TEST(Leaf, OptShortEq) {
    int i{};
    auto leaf = LeafBuilder().opt('i', "i", &i, "integer").commit();
    auto res = leaf.parse(mk_args("-i=2"));
    ASSERT_TRUE(res) << res.error().message();
    ASSERT_EQ(i, 2);
}

TEST(Leaf, OptLong) {
    int i{};
    auto leaf = LeafBuilder().opt("i", &i, "integer").commit();
    auto res = leaf.parse(mk_args("--i", "3"));
    ASSERT_TRUE(res) << res.error().message();
    ASSERT_EQ(i, 3);
}

TEST(Leaf, OptLongEq) {
    int i{};
    auto leaf = LeafBuilder().opt("i", &i, "integer").commit();
    auto res = leaf.parse(mk_args("--i=4"));
    ASSERT_TRUE(res) << res.error().message();
    ASSERT_EQ(i, 4);
}

TEST(Leaf, ShortFlag) {
    bool b{};
    auto leaf = LeafBuilder().flag('b', "bool", &b, "flag").commit();
    auto res = leaf.parse(mk_args("-b"));
    ASSERT_TRUE(res) << res.error().message();
    ASSERT_TRUE(b);
}
TEST(Leaf, LongFlag) {
    bool b{};
    auto leaf = LeafBuilder().flag("bool", &b, "flag").commit();
    auto res = leaf.parse(mk_args("--bool"));
    ASSERT_TRUE(res) << res.error().message();
    ASSERT_TRUE(b);
}

