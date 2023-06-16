#include "cent/dm/reference.hpp"

#include <gtest/gtest.h>

#include <cent/dm.hpp>

namespace dtail = cent::dm::ref_detail;
using namespace std::literals::string_view_literals;

TEST(Utils, FindDigest) {
    auto str = "localhost/foo@sha256:deadbeef"sv;
    auto idx = dtail::digest_sep(str);
    ASSERT_EQ(idx, str.find("@"));
}

TEST(Utils, FindRefStop) {
    auto str1 = "localhost/foo@sha256:deadbeef"sv;
    auto str2 = "docker.io/foo:bar"sv;
    ASSERT_EQ(dtail::ref_sep(str1), str1.find('@'));
    ASSERT_EQ(dtail::ref_sep(str2), str2.find(':'));
}

TEST(Utils, FindTag) {
    auto idx = dtail::tag_sep("localhost/foo:bar");
    ASSERT_EQ(idx, "localhost/foo:bar"sv.find(":"));
}
