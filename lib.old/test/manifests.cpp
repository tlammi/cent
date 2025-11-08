/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "cent/manifest.hpp"
#include "cent/manifest_list.hpp"

using namespace nlohmann::literals;

const auto MANIFEST_LIST_JSON = R"(
{
    "manifests": [
        {
            "digest": "sha256:a0a45bd8c6c4acd6967396366f01f2a68f73406327285edc5b7b07cb1cf073db",
            "mediaType": "application/vnd.docker.distribution.manifest.v2+json",
            "platform": {
                "architecture": "amd64",
                "os": "linux"
            },
            "size": 529
        },
        {
            "digest": "sha256:d23aa8e657602b0991be01bc98845f0039044e0274be15e0cff1ae330ab013b8",
            "mediaType": "application/vnd.docker.distribution.manifest.v2+json",
            "platform": {
                "architecture": "arm",
                "os": "linux",
                "variant": "v7"
            },
            "size": 529
        },
        {
            "digest": "sha256:b18dbe0837fd555c7028af6a1281ffa4fd1b5ffd835968f1009fd4cf9dfeaec3",
            "mediaType": "application/vnd.docker.distribution.manifest.v2+json",
            "platform": {
                "architecture": "arm64",
                "os": "linux",
                "variant": "v8"
            },
            "size": 529
        },
        {
            "digest": "sha256:189ed3d4e1b2d68cb23cddd5b25351c45e0cf734f96cfe6faeb7502e5339b617",
            "mediaType": "application/vnd.docker.distribution.manifest.v2+json",
            "platform": {
                "architecture": "ppc64le",
                "os": "linux"
            },
            "size": 529
        },
        {
            "digest": "sha256:e75cfa7ef5d58be253efa7d0fd4e027453d426734953dd39dd0d195705180855",
            "mediaType": "application/vnd.docker.distribution.manifest.v2+json",
            "platform": {
                "architecture": "riscv64",
                "os": "linux"
            },
            "size": 529
        },
        {
            "digest": "sha256:79b29fd5d282f6fa2b03eb47235ab8aaaf1c360d7ebcfb82ba3af8a6eb52eebe",
            "mediaType": "application/vnd.docker.distribution.manifest.v2+json",
            "platform": {
                "architecture": "s390x",
                "os": "linux"
            },
            "size": 529
        }
    ],
    "mediaType": "application/vnd.docker.distribution.manifest.list.v2+json",
    "schemaVersion": 2
}
)"_json;

const auto MANIFEST_JSON = R"(
{
   "schemaVersion": 2,
   "mediaType": "application/vnd.docker.distribution.manifest.v2+json",
   "config": {
      "mediaType": "application/vnd.docker.container.image.v1+json",
      "size": 1463,
      "digest": "sha256:a0ce5a295b637a10bc329ded296a0c895e5e56e7c5e674188d423e213b0d213e"
   },
   "layers": [
      {
         "mediaType": "application/vnd.docker.image.rootfs.diff.tar.gzip",
         "size": 28572685,
         "digest": "sha256:675920708c8bf10fbd02693dc8f43ee7dbe0a99cdfd55e06e6f1a8b43fd08e3f"
      }
   ]
}
)"_json;
TEST(Json, ManifestList) {
    cent::ManifestList manifest_list{MANIFEST_LIST_JSON};
}

TEST(Json, Manifest) { cent::Manifest manifest{MANIFEST_JSON}; }
