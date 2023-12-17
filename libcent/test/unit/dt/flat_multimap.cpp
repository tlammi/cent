#include <gtest/gtest.h>

#include <cent/dt.hpp>

template <class K, class V>
using Map = cent::FlatMultimap<K, V>;

TEST(Ctor, Default) {
    Map<int, int> m{};
    ASSERT_TRUE(m.empty());
}

TEST(Ctor, Values) {
    Map<int, int> m{{1, 1}, {2, 2}, {3, 3}};
    ASSERT_FALSE(m.empty());
}

TEST(Iter, Empty) {
    size_t count = 0;
    Map<int, int> m{};
    for (const auto& p : m) {
        (void)p;
        ++count;
    }
    ASSERT_EQ(count, 0);
}

TEST(Iter, Values) {
    Map<int, int> m{{1, 2}, {2, 3}, {3, 4}};
    size_t count = 0;
    for (const auto& [k, v] : m) {
        ASSERT_EQ(k, count + 1);
        ASSERT_EQ(v, count + 2);
        ++count;
    }
    ASSERT_EQ(count, 3);
}

TEST(MutIter, Empty) {
    size_t count = 0;
    Map<int, int> m{};
    for (auto& p : m) {
        (void)p;
        ++count;
    }
    ASSERT_EQ(count, 0);
}

TEST(MutIter, Values) {
    Map<int, int> m{{1, 2}, {2, 3}, {3, 4}};
    for (auto& [k, v] : m) {
        ++k;
        v += 2;
    }

    size_t count = 0;
    for (const auto& [k, v] : m) {
        ASSERT_EQ(k, count + 2);
        ASSERT_EQ(v, count + 4);
        ++count;
    }
    ASSERT_EQ(count, 3);
}

TEST(Find, Empty) {
    Map<int, int> m{};
    auto iter = m.find(1);
    auto const_iter = std::as_const(m).find(1);

    ASSERT_EQ(iter, m.end());
    ASSERT_EQ(const_iter, m.cend());
}

TEST(Find, Match) {
    Map<int, int> m{{1, 2}, {2, 3}};
    auto iter = m.find(1);
    ASSERT_NE(iter, m.end());
    ASSERT_EQ(iter->first, 1);
    ASSERT_EQ(iter->second, 2);
    auto const_iter = std::as_const(m).find(2);
    ASSERT_NE(const_iter, m.cend());
    ASSERT_EQ(const_iter->first, 2);
    ASSERT_EQ(const_iter->second, 3);
}

TEST(Contains, Simple) {
    Map<int, int> m{{1, 2}, {2, 3}};
    ASSERT_FALSE(m.contains(0));
    ASSERT_TRUE(m.contains(1));
    ASSERT_TRUE(m.contains(2));
    ASSERT_FALSE(m.contains(3));
}

TEST(Contains, Conversion) {
    Map<std::string, int> m{{"foo", 1}, {"bar", 2}, {"foo", 3}};
    ASSERT_TRUE(m.contains("foo"));
    ASSERT_TRUE(m.contains("bar"));
    ASSERT_FALSE(m.contains("baz"));
}

TEST(Count, Simple) {
    Map<int, int> m{{1, 2}, {1, 3}, {2, 3}, {3, 4}};
    ASSERT_EQ(m.count(0), 0);
    ASSERT_EQ(m.count(1), 2);
    ASSERT_EQ(m.count(2), 1);
    ASSERT_EQ(m.count(3), 1);
    ASSERT_EQ(m.count(4), 0);
}

TEST(Count, Conversion) {
    Map<std::string, int> m{{"foo", 1}, {"bar", 2}, {"foo", 3}};
    ASSERT_EQ(m.count("foo"), 2);
    ASSERT_EQ(m.count("bar"), 1);
    ASSERT_EQ(m.count("baz"), 0);
}

