/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

#include "cent/drv/context.hpp"
#include "cent/logs.hpp"
#include "cent/raise.hpp"
#include "cent/strutil.hpp"

// TODO: Use other drivers for accessing files
#include <fstream>
#include <sstream>

namespace cent::drv {
namespace {

struct SubId {
    uint64_t uid;
    uint64_t start;
    uint64_t count;
};

SubId parse_subid(std::string_view line) {
    std::string_view user{};
    std::string_view start{};
    std::string_view count{};
    std::string_view rest{};
    std::tie(user, rest) = split_left(line, ":");
    std::tie(start, count) = split_left(rest, ":");

    std::stringstream ss{};
    SubId id{};
    ss << start;
    ss >> id.start;
    ss.clear();
    ss << count;
    ss >> id.count;
    ss.clear();
    ss << user;
    ss >> id.uid;
    if (!ss) {
        std::string userstr{user};
        auto* pw = getpwnam(userstr.c_str());
        id.uid = pw->pw_uid;
    }
    return id;
}

std::vector<IdMap> id_maps(const char* file) {
    std::ifstream subuid{file};
    std::string data{std::istreambuf_iterator<char>(subuid),
                     std::istreambuf_iterator<char>()};
    auto lines = split(data, "\n");
    auto myuid = getuid();
    for (const auto& line : lines) {
        SubId id = parse_subid(line);
        if (id.uid == myuid) {
            return {IdMap{0, myuid, 1}, IdMap{1, id.start, id.count}};
        }
    }
    raise("No ID map found for the current user in ", file);
}

}  // namespace

class DefaultContext final : public Context {
 public:
    stdfs::path storage_path() override {
        auto* home = std::getenv("HOME");
        if (!home) raise("foo");
        stdfs::path path{home};
        return path / ".cent/storage";
    }
    stdfs::path workspace_path() override {
        auto* home = std::getenv("HOME");
        if (!home) raise("No HOME");
        stdfs::path path{home};
        return path / ".cent/workspace";
    }

    std::vector<IdMap> uid_maps() override { return id_maps("/etc/subuid"); }
    std::vector<IdMap> gid_maps() override { return id_maps("/etc/subgid"); }
};
std::unique_ptr<Context> default_context() {
    return std::make_unique<DefaultContext>();
}
}  // namespace cent::drv