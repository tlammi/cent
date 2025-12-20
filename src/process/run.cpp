#include "run.hpp"

#include <linux/sched.h>
#include <sched.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cassert>
#include <error.hpp>
#include <ranges>
#include <vector>

#include "process/search_path.hpp"

namespace cent::process {
namespace {

struct Deleter {
    void operator()(char** arr) {
        auto** ptr = &arr[0];
        while (*ptr) {
            delete *ptr;
            ++ptr;
        }
    }
};

using ArgArr = std::unique_ptr<char*[], Deleter>;

auto alloc_arg_arr(std::span<const std::string_view> s) {
    auto out = ArgArr(new char*[s.size() + 1]());
    for (size_t i = 0; i < s.size(); ++i) {
        out[i] = new (std::nothrow) char[s[i].size() + 1]();
        std::copy(s[i].begin(), s[i].end(), out[i]);
    }
    return out;
}

// Null terminated array of char* that can be passed to execve etc.
class Args {
 public:
    explicit Args(std::span<const std::string_view> s)
        : m_args(alloc_arg_arr(s)) {}

    Args(const Args&) = delete;
    Args& operator=(const Args&) = delete;

    Args(Args&&) noexcept = default;
    Args& operator=(Args&&) noexcept = default;
    ~Args() = default;

    char* const* args() const noexcept { return m_args.get(); }

 private:
    ArgArr m_args;
};
}  // namespace

int run(std::span<std::string_view> cmd, const RunOptions& opts) {
    if (cmd.size() == 0)
        raise(ErrorCode::InvalidArgument,
              "Need at least one argument in \"cmd\" for run");
    struct clone_args cloneargs{
        .flags = 0,
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

    auto pid = syscall(SYS_clone3, &cloneargs, sizeof(cloneargs));
    if (pid == 0) {
        auto args = Args(cmd);
        auto env = Args({});
        auto path = search_path(cmd[0]);
        (void)execve(path.c_str(), args.args(), env.args());
        raise_errno();
    }
    int status{};
    ::waitpid(pid, &status, 0);
    return status;
}

}  // namespace cent::process
