#include <gtest/gtest.h>

#include <cent/data/image_index.hpp>
#include <rfl/json.hpp>

using namespace std::literals;

constexpr auto SAMPLE_ENTRY = R"({
   "annotations": {
     "com.docker.official-images.bashbrew.arch": "amd64",
     "org.opencontainers.image.base.name": "scratch",
     "org.opencontainers.image.created": "2025-10-08T21:18:55Z",
     "org.opencontainers.image.revision": "4dc13cbc7caffe03c98aa99f28e27c2fb6f7e74d",
     "org.opencontainers.image.source": "https://github.com/alpinelinux/docker-alpine.git#4dc13cbc7caffe03c98aa99f28e27c2fb6f7e74d:x86_64",
     "org.opencontainers.image.url": "https://hub.docker.com/_/alpine",
     "org.opencontainers.image.version": "3.22.2"
   },
   "digest": "sha256:85f2b723e106c34644cd5851d7e81ee87da98ac54672b29947c052a45d31dc2f",
   "mediaType": "application/vnd.oci.image.manifest.v1+json",
   "platform": {
     "architecture": "amd64",
     "os": "linux"
   },
   "size": 1022
 })";

TEST(Parse, Entry) {
    auto res = rfl::json::read<cent::data::ImageIdxEntry>(SAMPLE_ENTRY);
    ASSERT_TRUE(res) << res.error().what();
    ASSERT_EQ(res->annotations.size(), 7);
    ASSERT_EQ(
        res->digest,
        "sha256:"
        "85f2b723e106c34644cd5851d7e81ee87da98ac54672b29947c052a45d31dc2f");
    ASSERT_EQ(res->platform.architecture, "amd64");
}

constexpr auto SAMPLE_DATA = R"({
  "manifests": [
    {
      "annotations": {
        "com.docker.official-images.bashbrew.arch": "amd64",
        "org.opencontainers.image.base.name": "scratch",
        "org.opencontainers.image.created": "2025-10-08T21:18:55Z",
        "org.opencontainers.image.revision": "4dc13cbc7caffe03c98aa99f28e27c2fb6f7e74d",
        "org.opencontainers.image.source": "https://github.com/alpinelinux/docker-alpine.git#4dc13cbc7caffe03c98aa99f28e27c2fb6f7e74d:x86_64",
        "org.opencontainers.image.url": "https://hub.docker.com/_/alpine",
        "org.opencontainers.image.version": "3.22.2"
      },
      "digest": "sha256:85f2b723e106c34644cd5851d7e81ee87da98ac54672b29947c052a45d31dc2f",
      "mediaType": "application/vnd.oci.image.manifest.v1+json",
      "platform": {
        "architecture": "amd64",
        "os": "linux"
      },
      "size": 1022
    }
  ],
  "mediaType": "application/vnd.oci.image.index.v1+json",
  "schemaVersion": 2
})"sv;

TEST(ImageIndex, Parse) {
    auto res = cent::data::parse_image_index(SAMPLE_DATA);
    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(res[0].platform.architecture, "amd64");
}
