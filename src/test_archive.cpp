#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "archive.hpp"
#include "util/make_array.hpp"

namespace arc = cent::archive;

template <class... Ts>
consteval auto make_byte_array(Ts&&... ts) {
    return cent::util::make_array<std::byte>(
        std::byte(std::forward<Ts>(ts))...);
}

// .tar.gz with only empty.txt
constexpr auto MINIMAL_ARCHIVE = make_byte_array(
    0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xed, 0xce,
    0x31, 0x0a, 0x02, 0x31, 0x14, 0x04, 0xd0, 0x1c, 0x25, 0x27, 0x90, 0xc4,
    0x4d, 0xdc, 0xf3, 0x6c, 0x61, 0x21, 0x18, 0x10, 0x8d, 0xa0, 0xb7, 0x77,
    0x95, 0xc5, 0x4e, 0xac, 0x76, 0xab, 0xf7, 0x9a, 0x81, 0x99, 0x5f, 0xfc,
    0x63, 0xbb, 0xf4, 0xe7, 0xae, 0x3f, 0x7a, 0x58, 0x4f, 0x9a, 0x1d, 0x4a,
    0x79, 0x67, 0x1e, 0x6b, 0x5a, 0x32, 0x7f, 0xfa, 0x45, 0xc8, 0x35, 0xe7,
    0xb1, 0xd4, 0x52, 0xd2, 0x10, 0x52, 0xde, 0x0f, 0x73, 0x15, 0xd3, 0x8a,
    0x3f, 0x7d, 0xdd, 0x6f, 0x7d, 0xba, 0xc6, 0x18, 0xfa, 0x79, 0x6a, 0xed,
    0xf4, 0xfb, 0xee, 0xdf, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x1b, 0x7a, 0x01, 0xff, 0x9c, 0x03, 0xd7, 0x00, 0x28, 0x00,
    0x00);

TEST(Ctor, Default) {
    auto a = arc::Archive();
    (void)a;
}

TEST(Ctor, Buffer) {
    auto a = arc::Archive(MINIMAL_ARCHIVE);
    (void)a;
}

TEST(Ctor, Range) {
    auto range = MINIMAL_ARCHIVE | std::views::chunk(10);
    auto a = arc::Archive(range);
    (void)a;
}

TEST(Buffer, Meta) {
    auto a = arc::Archive(MINIMAL_ARCHIVE);
    ASSERT_EQ(a.compression_name(), "gzip");
}

TEST(Buffer, IterateNames) {
    auto a = arc::Archive(MINIMAL_ARCHIVE);
    auto names = a | std::views::transform([](const auto& e) {
                     return std::string(e.path().c_str());
                 }) |
                 std::ranges::to<std::vector>();
    ASSERT_THAT(names, testing::ElementsAre("empty.txt"));
}

TEST(Buffer, IterateData) {
    auto a = arc::Archive(MINIMAL_ARCHIVE);
    auto it = a.begin();
    auto& e = *it;
    std::vector<std::byte> res{};
    e >> res;
    ASSERT_TRUE(res.empty());
}

TEST(Range, Meta) {
    auto range = MINIMAL_ARCHIVE | std::views::chunk(10);
    auto a = arc::Archive(range);
    ASSERT_EQ(a.compression_name(), "gzip");
}

TEST(Range, IterateNames) {
    auto range = MINIMAL_ARCHIVE | std::views::chunk(7);
    auto a = arc::Archive(range);
    auto names = a | std::views::transform([](const auto& e) {
                     return std::string(e.path().c_str());
                 }) |
                 std::ranges::to<std::vector>();
    ASSERT_THAT(names, testing::ElementsAre("empty.txt"));
}

TEST(Range, IterateData) {
    auto range = MINIMAL_ARCHIVE | std::views::chunk(5);
    auto a = arc::Archive(range);
    auto it = a.begin();
    auto& e = *it;
    std::vector<std::byte> res{};
    e >> res;
    ASSERT_TRUE(res.empty());
}
