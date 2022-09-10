#include <gtest/gtest.h>

#include "cent/storage/images.hpp"

using cent::storage::Images;
using namespace nlohmann::literals;

const auto EMPTY_JSON = R"(
[]
)"_json;

TEST(Images, Ctor) { Images i{EMPTY_JSON}; }