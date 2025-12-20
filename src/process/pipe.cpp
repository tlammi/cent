#include "pipe.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cassert>

#include "error.hpp"

namespace cent::process {
namespace detail {
PipeImpl::~PipeImpl() { close(); }

void PipeImpl::close() {
    if (m_fd) ::close(m_fd);
}

void PipeImpl::write(std::span<const std::byte> data) {
    assert(m_fd);
    if (data.size() > MAX_PACKET)
        raise(ErrorCode::InvalidArgument, "Buffer size exceeds maximum size");
    ::write(m_fd, data.data(), data.size());
}

size_t PipeImpl::read(std::span<std::byte> data) {
    assert(m_fd);
    if (data.size() > MAX_PACKET)
        raise(ErrorCode::InvalidArgument, "Buffer size exceeds maximum size");
    auto res = ::read(m_fd, data.data(), data.size());
    if (res < 0) raise_errno();
    return res;
}

std::pair<int, int> open_pipe(bool packet_mode) {
    int pipefd[2]{};
    auto res = pipe2(pipefd, packet_mode ? O_DIRECT : 0);
    if (res < 0) raise_errno();
    return {pipefd[0], pipefd[1]};
}

}  // namespace detail
}  // namespace cent::process
