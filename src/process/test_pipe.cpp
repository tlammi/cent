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
