#include <gtest/gtest.h>

#include <cent/data/manifest.hpp>
#include <string_view>

using namespace std::literals;

constexpr auto DATA = R"({
  "schemaVersion": 2,
  "mediaType": "application/vnd.oci.image.manifest.v1+json",
  "config": {
    "mediaType": "application/vnd.oci.image.config.v1+json",
    "digest": "sha256:706db57fb2063f39f69632c5b5c9c439633fda35110e65587c5d85553fd1cc38",
    "size": 581
  },
  "layers": [
    {
      "mediaType": "application/vnd.oci.image.layer.v1.tar+gzip",
      "digest": "sha256:2d35ebdb57d9971fea0cac1582aa78935adf8058b2cc32db163c98822e5dfa1b",
      "size": 3802452
    }
  ],
  "annotations": {
    "com.docker.official-images.bashbrew.arch": "amd64",
    "org.opencontainers.image.base.name": "scratch",
    "org.opencontainers.image.created": "2025-10-08T11:04:56Z",
    "org.opencontainers.image.revision": "4dc13cbc7caffe03c98aa99f28e27c2fb6f7e74d",
    "org.opencontainers.image.source": "https://github.com/alpinelinux/docker-alpine.git#4dc13cbc7caffe03c98aa99f28e27c2fb6f7e74d:x86_64",
    "org.opencontainers.image.url": "https://hub.docker.com/_/alpine",
    "org.opencontainers.image.version": "3.22.2"
  }
})"sv;

TEST(Parse, Sample) {
    auto res = cent::data::parse_manifest(DATA).unpack();
    ASSERT_EQ(res.layers.size(), 1);
    ASSERT_EQ(res.annotations.size(), 7);
    ASSERT_EQ(
        res.config.digest,
        "sha256:"
        "706db57fb2063f39f69632c5b5c9c439633fda35110e65587c5d85553fd1cc38");
}
