#include "stream.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "error.hpp"

namespace cent::io {
namespace {
auto get_size(int fd) -> size_t {
    struct stat st{};
    if (fstat(fd, &st) < 0) {
        if (errno == EBADF) return UNKNOWN_SIZE;
        raise_errno();
    }
    return st.st_size;
}

Fd open_ro(const char* path) {
    if (auto fd = ::open(path, O_RDONLY, 0); fd >= 0) return Fd{fd};
    raise_errno();
}

Fd open_wr(const char* path) {
    if (auto fd = ::open(path, O_WRONLY, 0); fd >= 0) return Fd{fd};
    raise_errno();
}

std::filesystem::path tmp_path(std::filesystem::path tgt) {
    tgt += ".tmp";
    return tgt;
}

Fd open_wr_tmp(std::filesystem::path tgt) {
    return open_wr(tmp_path(tgt).c_str());
}

}  // namespace

OFStream::OFStream(const char* path) : m_fd(open_wr(path)) {}

size_t OFStream::size() { return get_size(m_fd.fd()); }

size_t OFStream::write(std::span<const std::byte> buf) {
    auto res = ::write(m_fd.fd(), buf.data(), buf.size());
    if (res < 0) raise_errno();
    return res;
}

AtomicOFStream::AtomicOFStream(std::filesystem::path path)
    : m_tgt(std::move(path)), m_fd(open_wr_tmp(m_tgt.c_str())) {}

size_t AtomicOFStream::size() { return get_size(m_fd.fd()); }

size_t AtomicOFStream::write(std::span<const std::byte> buf) {
    auto res = ::write(m_fd.fd(), buf.data(), buf.size());
    if (res < 0) raise_errno();
    return res;
}
void AtomicOFStream::commit() {
    ::fsync(m_fd.fd());
    m_fd.close();
    if (::rename(tmp_path(m_tgt).c_str(), m_tgt.c_str()) < 0) raise_errno();
}

size_t IFStream::size() { return get_size(m_fd.fd()); }

size_t IFStream::read(std::span<std::byte> buf) {
    auto res = ::read(m_fd.fd(), buf.data(), buf.size());
    if (res < 0) raise_errno();
    return res;
}

}  // namespace cent::io
