/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include <errno.h>
#include <string.h>
#include <sys/file.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fstream>

#include "cent/drv/file_system.hpp"
#include "cent/drv/process.hpp"
#include "cent/logs.hpp"
#include "cent/raise.hpp"
#include "cent/strutil.hpp"

namespace cent::drv {
namespace {

bool check_user_exec(const struct stat& st, uid_t myuid, stdfs::perms perms) {
    return (myuid == st.st_uid) &&
           ((perms & stdfs::perms::owner_exec) != stdfs::perms::none);
}

bool check_group_exec(const struct stat& st, gid_t mygid, stdfs::perms perms) {
    return (mygid == st.st_gid) &&
           ((perms & stdfs::perms::group_exec) != stdfs::perms::none);
}

bool check_other_exec(stdfs::perms perms) {
    return (perms & stdfs::perms::others_exec) != stdfs::perms::none;
}

bool can_exec(const stdfs::path& p, stdfs::perms perms) {
    struct stat st {};
    int res = stat(p.c_str(), &st);
    if (res) raise("stat: ", res);
    return check_user_exec(st, geteuid(), perms) ||
           check_group_exec(st, getegid(), perms) || check_other_exec(perms);
}

}  // namespace

class LinuxFileSystemApi final : public FileSystem {
 public:
    Result mkdir(const stdfs::path& path, bool parents) override {
        std::error_code ec;
        if (parents) {
            bool res = stdfs::create_directories(path, ec);
            if (!res) return Result{ec.value(), ec.message()};
            return Result{0, ""};
        }
        bool res = stdfs::create_directory(path, ec);
        if (!res) return Result{ec.value(), ec.message()};
        return Result{0, ""};
    }

    std::unique_ptr<std::iostream> open_file(
        const stdfs::path& path, std::ios_base::openmode mode) override {
        return std::make_unique<std::fstream>(path, mode);
    }

    bool exists(const stdfs::path& path) override {
        return stdfs::exists(path);
    }

    stdfs::path find_program(std::string_view program) override {
        std::string_view env_path = std::getenv("PATH");
        auto path_parts = split(env_path, ":");
        for (const auto part : path_parts) {
            stdfs::path path{part};
            for (const auto& entry : stdfs::directory_iterator(path)) {
                if (entry.path().filename() == program) {
                    if (can_exec(entry.path(), entry.status().permissions())) {
                        return entry.path();
                    }
                }
            }
        }
        return stdfs::path();
    }
    int run_program(const stdfs::path& cmd,
                    const std::vector<std::string_view>& args,
                    const std::vector<std::string_view>& env, std::string* out,
                    std::string* err, const std::string* in) override {
        Process p{};
        std::string cmdstr = cmd.string();
        std::vector<std::string_view> allargs{cmdstr};
        allargs.insert(allargs.end(), args.begin(), args.end());
        p.args(allargs);
        p.env(env);
        if (out) p.out(*out);
        if (err) p.err(*err);
        if (in) p.in(in);
        return p.run();
    }

    int lock_file(const stdfs::path& path) override {
        int fd = open(path.c_str(), O_RDWR | O_CREAT, 0666);
        int rc = flock(fd, LOCK_EX);
        if (rc) { raise("Could not lock", path); }
        return fd;
    }

    void unlock_file(int fd) override { close(fd); }

    void overlayfs_mount(const std::vector<stdfs::path>& paths,
                         const stdfs::path& upper, const stdfs::path& work,
                         const stdfs::path& dst) final {
        std::string options{"lowerdir="};
        options += join(paths, ":");
        if (!upper.empty()) {
            options += ",upperdir=";
            options += upper;
        }
        if (!work.empty()) {
            options += ",workdir=";
            options += work;
        }
        logs::debug("using options: ", options);
        int res = mount("overlay", dst.c_str(), "overlay", 0, options.c_str());
        if (res) raise(strerror(errno));
    }
};

std::unique_ptr<FileSystem> default_file_system() {
    return std::make_unique<LinuxFileSystemApi>();
}
}  // namespace cent::drv