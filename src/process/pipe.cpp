#include "pipe.hpp"

#include <fcntl.h>
#include <unistd.h>

#include "error.hpp"

namespace cent::process {
namespace detail {
PipeImpl::~PipeImpl() {
    if (m_fd) ::close(m_fd);
}
void PipeImpl::write(std::span<const std::byte> data) {
    if (data.size() > MAX_PACKET)
        raise(ErrorCode::InvalidArgument, "Buffer size exceeds maximum size");
    ::write(m_fd, data.data(), data.size());
}

void PipeImpl::read(std::span<std::byte> data) {
    if (data.size() > MAX_PACKET)
        raise(ErrorCode::InvalidArgument, "Buffer size exceeds maximum size");
    ::read(m_fd, data.data(), data.size());
}

std::pair<int, int> open_pipe(bool packet_mode) {
    int pipefd[2]{};
    auto res = pipe2(pipefd, packet_mode ? O_DIRECT : 0);
    if (res < 0) raise_errno();
    return {pipefd[0], pipefd[1]};
}

}  // namespace detail
}  // namespace cent::process
