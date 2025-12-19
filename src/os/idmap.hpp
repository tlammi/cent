#pragma once

#include <sys/types.h>

#include <string_view>
#include <vector>

namespace cent::os {

struct IdRange {
    uid_t start{};
    uid_t count{};
};

std::vector<IdRange> subuid_ranges(const char* username, uid_t uid);
std::vector<IdRange> subuid_ranges(const char* username);
std::vector<IdRange> subuid_ranges(uid_t uid);
std::vector<IdRange> subuid_ranges();

std::vector<IdRange> subgid_ranges(const char* username, uid_t uid);
std::vector<IdRange> subgid_ranges(const char* username);
std::vector<IdRange> subgid_ranges(uid_t uid);
std::vector<IdRange> subgid_ranges();

}  // namespace cent::os
