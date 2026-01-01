#include "fd.hpp"

#include "error.hpp"

namespace cent::io {
namespace {
int pidfd_open(pid_t pid, unsigned int flags) {
    auto res = syscall(SYS_pidfd_open, pid, flags);
    if (res < 0) raise(ErrorCode::Generic, "pidfd_open");
    return res;
}
}  // namespace

Fd::Fd(pidfd_t, pid_t pid) : m_fd(pidfd_open(pid, 0)) {}
}  // namespace cent::io
