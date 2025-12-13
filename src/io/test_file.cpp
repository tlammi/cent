#include <gtest/gtest.h>

#include "file.hpp"

namespace io = cent::io;

TEST(Ctor, Default) {
    auto f = io::File();
    ASSERT_FALSE(f.handle());
    ASSERT_ANY_THROW(f.fd());
}
