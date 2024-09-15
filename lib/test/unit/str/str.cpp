#include <gtest/gtest.h>

#include <cent/str.hpp>

TEST(Init, Default) { cent::Str{}; }
TEST(Init, StrLit) { cent::Str("foo"); }
TEST(Init, StdString) { cent::Str(std::string("foo")); }

