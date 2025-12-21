#include <algorithm>
#include <ranges>

#include "memmap.hpp"

namespace io = cent::io;

TEST(Ctor, DefaultRO) {
    auto map = io::MemMapRO();
    ASSERT_FALSE(map.data());
    ASSERT_EQ(map.size(), 0);
}

TEST(Ctor, DefaultRW) {
    auto map = io::MemMapRW();
    ASSERT_FALSE(map.data());
    ASSERT_EQ(map.size(), 0);
}

TEST(Access, TmpFileRO) {
    auto map = io::MemMapRO();
    {
        auto f = io::tmpfile().second;
        f.truncate(1024);
        map = io::MemMapRO(f);
    }
    auto all_zero =
        std::ranges::all_of(map, [](auto b) { return b == std::byte{0}; });
    ASSERT_TRUE(all_zero);
}

TEST(Access, TmpFileRW) {
    auto map = io::MemMapRW();
    auto data = std::views::iota(0, 1024) |
                std::views::transform([](auto v) { return std::byte(v); }) |
                std::ranges::to<std::vector<std::byte>>();
    {
        auto f = io::tmpfile().second;
        f.truncate(1024);
        map = io::MemMapRW(f);
        std::copy(data.begin(), data.end(), map.begin());
    }

    auto res = std::vector(map.begin(), map.end());
    ASSERT_EQ(res, data);
}
