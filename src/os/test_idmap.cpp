#include "detail/idmap.inl"
#include "idmap.hpp"

using namespace cent::os;
using namespace std::literals;

TEST(Split, Simple) {
    auto [user, start, count] = split_subid("foo:1:2"sv);
    ASSERT_EQ(user, "foo");
    ASSERT_EQ(start, "1");
    ASSERT_EQ(count, "2");
}

std::vector<IdRange> mk_idranges(std::initializer_list<IdRange> maps) {
    return {maps};
}

TEST(Normalize, Empty) {
    auto r = mk_idranges({});
    r = normalize(r);
    ASSERT_TRUE(r.empty());
}

TEST(Normalize, One) {
    auto r = mk_idranges({{0, 100}});
    ASSERT_EQ(normalize(r), r);
}

TEST(Normalize, NoOverlap) {
    auto r = mk_idranges({{1, 10}, {20, 10}});
    ASSERT_EQ(normalize(r), r);
}

TEST(Normalize, Overlap) {
    auto r = mk_idranges({{1, 100}, {50, 100}});
    auto expected = mk_idranges({{1, 149}});
    ASSERT_EQ(normalize(r), expected);
}

TEST(Normalize, MultiOverlap) {
    auto r = mk_idranges({{0, 10}, {5, 10}, {10, 10}});
    auto expected = std::vector<IdRange>{{0, 20}};
    ASSERT_EQ(normalize(r), expected);
}

TEST(Normalize, NotOrderedNoOverlap) {
    auto r = mk_idranges({{20, 10}, {0, 5}});
    auto expected = mk_idranges({{0, 5}, {20, 10}});
    ASSERT_EQ(normalize(r), expected);
}

TEST(Normalize, NotOrderedOverlap) {
    auto r = mk_idranges({{20, 10}, {0, 25}});
    auto expected = mk_idranges({{0, 30}});
    ASSERT_EQ(normalize(r), expected);
}
