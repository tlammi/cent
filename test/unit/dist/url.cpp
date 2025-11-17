#include <gtest/gtest.h>

#include <cent/dist/url.hpp>

TEST(FromName, Manifest) {
    auto nm = cent::Name("docker.io/library/alpine:latest");
    auto url = cent::dist::manifest_url(nm);
    ASSERT_EQ(url, cent::dist::UrlView(
                       "https://docker.io/library/alpine/manifests/latest"));
}
