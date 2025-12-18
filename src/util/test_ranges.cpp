#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ranges.hpp"

namespace rv = std::ranges::views;
namespace util = cent::util;
using namespace std::literals;

TEST(ToArray, One) {
    auto res = "foo"sv | rv::split(':') | rv::transform([](auto v) {
                   return std::string_view(v.data(), v.size());
               }) |
               util::to_array<std::string_view, 1>();
    ASSERT_EQ(res.at(0), "foo");
}

TEST(ToArray, Multiple) {
    auto res = "foo:bar:baz"sv | rv::split(':') |
               util::to_array<std::string_view, 3>();
    ASSERT_THAT(res, testing::ElementsAre("foo", "bar", "baz"));
}

TEST(ToArray, TooFew) {
    auto fn = [] {
        return "foo"sv | rv::split(':') | util::to_array<std::string_view, 2>();
    };
    ASSERT_ANY_THROW(fn());
}
TEST(ToArray, TooMany) {
    auto fn = [] {
        return "foo:bar:baz"sv | rv::split(':') |
               util::to_array<std::string_view, 2>();
    };
    ASSERT_ANY_THROW(fn());
}
