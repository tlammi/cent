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
