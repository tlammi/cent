#include <gtest/gtest.h>

#include <cent/app/args.hpp>

constexpr cent::app::Opt make_opt(std::string_view s) {
    if (s.starts_with("--")) return {cent::app::OptType::Long, s.substr(2)};
    if (s.starts_with('-')) return {cent::app::OptType::Short, s.substr(1)};
    return {cent::app::OptType::Val, s};
}

TEST(Cmp, EndEqEnd) {
    std::vector<const char*> args{};
    auto iter = cent::app::ArgIter<const char>();
    auto end = cent::app::ArgIter<const char>();
    ASSERT_EQ(iter, end);
}

TEST(C, IterNeEnd) {
    std::vector<const char*> args{"foo", "bar"};
    auto iter = cent::app::ArgIter(args.size(), args.data());
    auto end = cent::app::ArgIter<const char>();
    ASSERT_NE(iter, end);
}

TEST(Cmp, IterEqIter) {
    std::vector<const char*> args{"foo", "bar"};
    auto iter = cent::app::ArgIter(args.size(), args.data());
    auto iter2 = cent::app::ArgIter(args.size(), args.data());
    ASSERT_EQ(iter, iter2);
}

TEST(Cmp, IterationCount) {
    std::vector<const char*> args{"foo", "bar"};
    size_t count = 0;
    for (const auto arg : cent::app::args(args.size(), args.data())) {
        (void)arg;
        std::cerr << "count: " << count << '\n';
        ++count;
    }
    ASSERT_EQ(count, args.size());
}

TEST(Iter, LongFlagsOnly) {
    std::vector<const char*> args{"--foo", "--bar"};
    size_t i = 0;
    for (auto arg : cent::app::args(args.size(), args.data())) {
        auto exp = make_opt(args.at(i));
        ASSERT_EQ(arg, exp);
        ++i;
    }
}

TEST(Iter, SeparateShortFlags) {
    std::vector<const char*> args{"-f", "-b"};
    size_t i = 0;
    for (auto arg : cent::app::args(args.size(), args.data())) {
        auto exp = make_opt(args.at(i));
        ASSERT_EQ(arg, exp);
        ++i;
    }
}

TEST(Iter, LongAndShortFlags) {
    std::vector<const char*> args{"--foo", "-b"};
    size_t i = 0;
    for (auto arg : cent::app::args(args.size(), args.data())) {
        auto exp = make_opt(args.at(i));
        ASSERT_EQ(arg, exp);
        ++i;
    }
}

TEST(Iter, LongFlagWithEqual) {
    std::vector<const char*> args{"--foo=bar"};
    auto iter = cent::app::ArgIter(args.size(), args.data());
    auto exp = make_opt("--foo");
    ASSERT_EQ(*iter, exp);
    ++iter;
    exp = make_opt("bar");
    ASSERT_EQ(*iter, exp);
}

TEST(Iter, ShortFlagWithEqual) {
    std::vector<const char*> args{"-abc=value"};
    auto iter = cent::app::ArgIter(args.size(), args.data());
    ASSERT_EQ(*iter, make_opt("-a"));
    ++iter;
    ASSERT_EQ(*iter, make_opt("-b"));
    ++iter;
    ASSERT_EQ(*iter, make_opt("-c"));
    ++iter;
    ASSERT_EQ(*iter, make_opt("value"));
    ++iter;
    ASSERT_EQ(iter, cent::app::ArgIter<const char>());
}
