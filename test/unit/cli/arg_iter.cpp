#include <gtest/gtest.h>

#include <cent/cli/arg_iter.hpp>

TEST(Eq, Empty) {
    static constexpr cent::cli::ArgIter it{};
    static constexpr cent::cli::ArgIter it2{};
    ASSERT_EQ(it, it2);
}

TEST(Eq, One) {
    std::vector<const char*> args{"hello"};
    cent::cli::ArgIter it{args};
    cent::cli::ArgIter it2{args};
    ASSERT_EQ(it, it2);
}

TEST(Iterate, Simple) {
    std::vector<const char*> args{"hello", "world"};
    size_t idx = 0;
    for (auto arg : cent::cli::arg_range(args)) {
        ASSERT_EQ(arg, args.at(idx));
        ++idx;
    }
}

TEST(Iterate, Equal) {
    std::vector<const char*> args{"--foo=bar", "baz"};
    std::vector<const char*> expected{"--foo", "bar", "baz"};

    size_t idx = 0;
    for (auto arg : cent::cli::arg_range(args)) {
        ASSERT_EQ(arg, expected.at(idx))
            << arg << " vs " << expected.at(idx) << " at index " << idx;
        ++idx;
    }
}
