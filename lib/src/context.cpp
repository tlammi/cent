#include "cent/context.hpp"

#include <pwd.h>

#include "cent/drv/file_system_impl.hpp"
#include "cent/raise.hpp"
#include "cent/strutil.hpp"

namespace cent {

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
    auto subuid = drv::fs().open_file(file, std::ios_base::in);
    std::string data{std::istreambuf_iterator<char>(*subuid),
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

Context::Context(const Config& conf)
    : workspace_path{conf.workspace_path},
      storage_path{conf.storage_path},
      uid_maps{id_maps("/etc/subuid")},
      gid_maps{id_maps("/etc/subgid")} {}

}  // namespace cent