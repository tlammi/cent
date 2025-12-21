#include "pipe.hpp"

using cent::process::pipe;

TEST(Init, Default) {
    auto [reader, writer] = pipe<int>();
    (void)reader;
    (void)writer;
}

TEST(Init, CloseReader) {
    auto [r, w] = pipe<int>();
    r.close();
}

TEST(Init, CloseWriter) {
    auto [r, w] = pipe<int>();
    w.close();
}

TEST(RoundTrip, One) {
    auto [reader, writer] = pipe<int>();
    writer << 1;
    int res{};
    reader >> res;
    ASSERT_EQ(res, 1);
}

TEST(RoundTrip, Arr) {
    auto [reader, writer] = pipe<int[]>();
    auto data = std::vector<int>{1, 2, 3};
    writer << data;
    auto out = std::vector<int>(3, 0);
    reader >> out;
    ASSERT_EQ(out, data);
}

TEST(RoundTrip, Struct) {
    struct Foo {
        int a{};
        double b{};
        std::array<unsigned, 4> arr{};

        constexpr auto operator<=>(const Foo&) const noexcept = default;
    };

    auto data = Foo{.a = 1, .b = 10.0, .arr = {1, 2, 3, 4}};

    auto [r, w] = pipe<Foo>();
    w << data;
    auto res = Foo{};
    r >> res;
    ASSERT_EQ(data, res);
}
