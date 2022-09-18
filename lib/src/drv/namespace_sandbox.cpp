/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include <linux/sched.h>
#include <sched.h>
#include <signal.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

#include "cent/drv/sandbox.hpp"
#include "cent/logs.hpp"
#include "cent/raise.hpp"

namespace cent::drv {
namespace {

constexpr uint64_t CHILD_CONTINUE = 0x06;

void serialize_uidmap(std::stringstream& ss,
                      const std::vector<Sandbox::IdMap>& maps) {
    for (const auto& [in, out, count] : maps) {
        ss << " " << in << " " << out << " " << count;
    }
}

void map_ids(std::string_view mapper, pid_t child_pid,
             const std::vector<Sandbox::IdMap>& maps) {
    if (maps.empty()) return;
    std::stringstream ss;
    ss << mapper << " " << child_pid;
    serialize_uidmap(ss, maps);
    int res = system(ss.str().c_str());
    if (res) { raise("Failed to create mapping '", ss.str(), "': ", res); }
}
}  // namespace

class NamespaceSandbox final : public Sandbox {
 public:
    void set_uid_maps(std::vector<IdMap> map) override {
        m_uid_maps = std::move(map);
    }

    const std::vector<IdMap>& uid_maps() const override { return m_uid_maps; }

    void set_gid_maps(std::vector<IdMap> map) override {
        m_gid_maps = std::move(map);
    }

    const std::vector<IdMap>& gid_maps() const override { return m_gid_maps; }

    void fork(const std::function<void()>& func) override {
        clone_args cargs{
            .flags = CLONE_NEWUSER | CLONE_NEWNS,
            .pidfd = 0,
            .child_tid = 0,
            .parent_tid = 0,
            .exit_signal = SIGCHLD,
            .stack = 0,
            .stack_size = 0,
            .tls = 0,
            .set_tid = 0,
            .set_tid_size = 0,
            .cgroup = 0,
        };
        int fd = eventfd(0, 0);
        int pid = syscall(SYS_clone3, &cargs, sizeof(cargs));
        if (pid < 0) { raise("clone3()"); }
        if (pid == 0) {
            uint64_t msg;
            if (read(fd, &msg, sizeof(msg)) != sizeof(msg) ||
                msg != CHILD_CONTINUE)
                exit(EXIT_FAILURE);
            close(fd);
            func();
            exit(EXIT_SUCCESS);
        } else {
            logs::trace("Created sandbox process ", pid);
            map_ids("newuidmap", pid, m_uid_maps);
            map_ids("newgidmap", pid, m_gid_maps);
            write(fd, &CHILD_CONTINUE, sizeof(CHILD_CONTINUE));
            close(fd);
            int status;
            pid_t waited_pid = wait(&status);
            if (waited_pid != pid)
                raise("Caught a signal for an invalid process. Expected '", pid,
                      "', got '", waited_pid, '\'');
            if (!WIFEXITED(status)) {
                raise("Sandbox child process failed: ", WEXITSTATUS(status));
            }
            logs::trace("Sandbox child finished successfully");
        }
    }

 private:
    std::vector<IdMap> m_uid_maps{};
    std::vector<IdMap> m_gid_maps{};
};

std::unique_ptr<Sandbox> default_sandbox() {
    return std::make_unique<NamespaceSandbox>();
}
}  // namespace cent::drv