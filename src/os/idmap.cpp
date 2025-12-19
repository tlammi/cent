#include "idmap.hpp"

#include <ranges>
#include <vector>

#include "error.hpp"
#include "io/file.hpp"
#include "login.hpp"
#include "os/detail/idmap.inl"
#include "util/parse.hpp"
#include "util/split.hpp"

namespace cent::os {
namespace {

uid_t unpack(std::optional<uid_t> opt) {
    if (!opt) raise(ErrorCode::FormatError, "malformed /etc/sub{u,g}id");
    return *opt;
}

uid_t parse_uid(std::string_view s) {
    return unpack(util::parse_int<uid_t>(s));
}

std::vector<IdRange> subids_from_file(const std::filesystem::path& path,
                                      const char* username, uid_t uid) {
    using namespace std::views;
    auto uid_str = std::to_string(uid);
    auto f = io::open(path, io::openr);
    std::string data{};
    f >> data;

    auto filter_user = filter([&](const auto& elem) {
        const auto& [user, start, stop] = elem;
        return user == username || user == uid_str;
    });
    auto to_id_range = transform([](auto split_line) {
        auto [_, start, count] = split_line;
        return IdRange{
            .start = parse_uid(start),
            .count = parse_uid(count),
        };
    });
    return split_subids(data) | filter_user | to_id_range |
           std::ranges::to<std::vector>();
}

}  // namespace

std::vector<IdRange> subuid_ranges(const char* username, uid_t uid) {
    using namespace std::views;
    return subids_from_file("/etc/subuid", username, uid);
}

std::vector<IdRange> subuid_ranges(const char* username) {
    return subuid_ranges(username, os::getuid(username));
}

std::vector<IdRange> subuid_ranges(uid_t uid) {
    auto nm = os::getlogin(uid);
    return subuid_ranges(nm.c_str(), uid);
}

std::vector<IdRange> subuid_ranges() {
    auto user = os::getlogin();
    return subuid_ranges(user.c_str());
}

std::vector<IdRange> subgid_ranges(const char* username, uid_t uid) {
    using namespace std::views;
    return subids_from_file("/etc/subgid", username, uid);
}

std::vector<IdRange> subgid_ranges(const char* username) {
    return subgid_ranges(username, os::getuid(username));
}

std::vector<IdRange> subgid_ranges(uid_t uid) {
    auto nm = os::getlogin(uid);
    return subgid_ranges(nm.c_str(), uid);
}

std::vector<IdRange> subgid_ranges() {
    auto user = os::getlogin();
    return subgid_ranges(user.c_str());
}

}  // namespace cent::os
