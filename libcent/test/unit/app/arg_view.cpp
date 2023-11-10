#include <gtest/gtest.h>

#include <cent/app/arg_view.hpp>
#include <vector>

using cent::app::ArgView;

TEST(Ctor, Default) {
    ArgView<char> av{};
    ASSERT_FALSE(av);
    ASSERT_TRUE(av.empty());
    ASSERT_EQ(av.size(), 0);
    ASSERT_EQ(av.length(), 0);
}

TEST(Ctor, Values) {
    std::vector<const char*> args{"a", "b", "c"};
    ArgView av{args.size(), args.data()};
    ASSERT_FALSE(av.empty());
    ASSERT_TRUE(av);
    ASSERT_EQ(av.size(), 3);
    ASSERT_EQ(av.length(), 3);
}

TEST(RemovePrefix, Default) {
    std::vector<const char*> args{"a", "b", "c"};
    ArgView av{args.size(), args.data()};
    av.remove_prefix();
    ASSERT_EQ(av.size(), 2);
    ASSERT_EQ(av.front(), "b");
}

TEST(RemovePrefix, Multiple) {
    std::vector<const char*> args{"a", "b", "c"};
    ArgView av{args.size(), args.data()};
    av.remove_prefix(2);
    ASSERT_EQ(av.size(), 1);
    ASSERT_EQ(av.front(), "c");
}

