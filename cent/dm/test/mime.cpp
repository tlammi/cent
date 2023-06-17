#include <gtest/gtest.h>

#include <cent/core.hpp>
#include <cent/dm/mime.hpp>

using Mt = cent::dm::MimeType;

constexpr bool contains(cent::StaticStr str, cent::StaticStr substr) {
    return str.view().find(substr.view()) != std::string_view::npos;
}
TEST(Docker, ManifestList) {
    auto mime = cent::dm::docker_mime(Mt::ManifestList);
    ASSERT_TRUE(contains(mime, "manifest.list.v2"));
    ASSERT_TRUE(contains(mime, "docker"));
}

TEST(Oci, ManifestList) {
    auto mime = cent::dm::oci_mime(Mt::ManifestList);
    ASSERT_TRUE(contains(mime, "manifest.list.v2"));
}

TEST(Both, Eq) {
    auto do_check = [](Mt type) {
        EXPECT_EQ(cent::dm::docker_mime(type), cent::dm::mime(type, true));
        EXPECT_EQ(cent::dm::oci_mime(type), cent::dm::mime(type, false));
    };

    do_check(Mt::ManifestList);
}
