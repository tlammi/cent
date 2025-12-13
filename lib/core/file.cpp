#include <fcntl.h>
#include <sys/mman.h>

#include <cent/core/file.hpp>
#include <cent/error.hpp>
#include <cent/util/defer.hpp>

namespace cent {
namespace {

[[noreturn]] void raise_errno() {
    // TODO: Does this need to be more fine-grained?
    raise(ErrorCode::Generic, "{}", strerror(errno));
}

auto alloc_file(const std::filesystem::path& path, size_t size) {
    auto fd = open(path.c_str(), O_WRONLY | O_CREAT);
    if (fd < 0) raise_errno();
    auto cleanup = util::Defer([&] { close(fd); });
    auto seeked = lseek(fd, size, SEEK_SET);
    if (seeked < 0) raise_errno();
    auto buf = std::array<std::byte, 1>{std::byte{0}};
    auto wrote = write(fd, buf.data(), buf.size());
    if (wrote < 0) raise_errno();
    cleanup.cancel();
    return fd;
}
}  // namespace

class MemoryMappedOut final : public FileOut {
 public:
    MemoryMappedOut(const std::filesystem::path& path, size_t size) {
        auto fd = alloc_file(path, size);
        void* data = mmap(nullptr, size, PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
        if (data == MAP_FAILED) {
            // TODO: Does this need tob e more fine-grained?
            raise(ErrorCode::Generic, "{}", strerror(errno));
        }
        m_buf = std::span(reinterpret_cast<std::byte*>(data), size);
        m_ptr = m_buf.data();
    }
    ~MemoryMappedOut() override {
        if (m_buf.data()) {
            auto res = munmap(m_buf.data(), m_buf.size());
            // TODO: Logging?
            (void)res;
        }
    }

    std::span<std::byte> buffer() override { return {m_ptr, m_buf.size()}; }

    void write() override {
        // nop
        // In theory buffer() could return a subspan of the memory and this
        // could move the pointer.
    }

    void seek(long offset, SeekOrigin orig) override {
        using enum SeekOrigin;
        switch (orig) {
            case Set: m_ptr = &m_buf.data()[offset]; break;
            case Current: m_ptr += offset; break;
            case End: m_ptr = &m_buf.data()[m_buf.size() - offset]; break;
        }
    }

 private:
    std::span<std::byte> m_buf{};
    std::byte* m_ptr{};
};

AnyFileOut write_memory_mapped(const std::filesystem::path& path, size_t size) {
    return AnyFileOut{std::make_unique<MemoryMappedOut>(path, size)};
}
}  // namespace cent
