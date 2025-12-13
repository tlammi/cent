#include "memmap.hpp"

#include <sys/mman.h>

#include "error.hpp"

namespace cent::io {
namespace {

std::span<const std::byte> mmap_ro(File& f) {
    auto sz = f.size();
    const auto* addr = mmap(nullptr, sz, PROT_READ, MAP_SHARED, f.fd(), 0);
    if (addr == MAP_FAILED) raise_errno();
    return {reinterpret_cast<const std::byte*>(addr), size_t(sz)};
}

std::span<std::byte> mmap_rw(File& f) {
    auto sz = f.size();
    auto* addr =
        mmap(nullptr, sz, PROT_READ | PROT_WRITE, MAP_SHARED, f.fd(), 0);
    if (addr == MAP_FAILED) raise_errno();
    return {reinterpret_cast<std::byte*>(addr), size_t(sz)};
}

}  // namespace

MemMapRO::MemMapRO(File& f) : m_span(mmap_ro(f)) {}
MemMapRO::~MemMapRO() {
    if (m_span.data()) {
        // Const cast since this was initially a void* returned by mmap()
        munmap(const_cast<std::byte*>(m_span.data()), m_span.size());
    }
}

MemMapRW::MemMapRW(File& f) : m_span(mmap_rw(f)) {}
MemMapRW::~MemMapRW() {
    if (m_span.data()) munmap(m_span.data(), m_span.size());
}
}  // namespace cent::io
