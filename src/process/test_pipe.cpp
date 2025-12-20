#include <gtest/gtest.h>

#include "pipe.hpp"

using cent::process::pipe;

TEST(Init, Default) {
    auto [reader, writer] = pipe<int>();
    (void)reader;
    (void)writer;
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
