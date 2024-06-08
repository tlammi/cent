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
