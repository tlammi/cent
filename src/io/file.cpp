#include "file.hpp"

#include <cassert>

#include "error.hpp"
#include "util/defer.hpp"

namespace cent::io {

namespace {

auto convert_orig(SeekOrigin orig) {
    using enum SeekOrigin;
    switch (orig) {
        case Set: return SEEK_SET;
        case Current: return SEEK_CUR;
        case End: return SEEK_END;
    }
    std::unreachable();
}
}  // namespace
File::~File() {
    if (m_str) fclose(m_str);
}
FILE* File::handle() const noexcept { return m_str; }

int File::fd() {
    assert(m_str);
    auto f = fileno(m_str);
    if (f < 0) raise_errno();
    return f;
}

void File::seek(int64_t offset, SeekOrigin orig) {
    auto res = fseeko(m_str, offset, convert_orig(orig));
    if (res) raise(ErrorCode::Generic, "fseeko()");
}

int64_t File::position() const {
    auto res = ftello(m_str);
    if (res < 0) raise_errno();
    return res;
}

int64_t File::size() {
    auto initial = position();
    seek(0, SeekOrigin::End);
    auto last = position();
    seek(initial, SeekOrigin::Set);
    return last;
}
void File::truncate(int64_t size) {
    auto res = ftruncate64(fd(), size);
    if (res < 0) raise_errno();
}

size_t FileI::read(std::span<std::byte> buf) {
    auto res = fread(buf.data(), sizeof(std::byte), buf.size(), handle());
    if (res < buf.size() && !std::feof(handle())) [[unlikely]]
        raise(ErrorCode::Generic, "fread()");
    return res;
}

void FileO::write(std::span<const std::byte> buf) {
    assert(handle());
    auto res = fwrite(buf.data(), sizeof(std::byte), buf.size(), handle());
    if (res < buf.size()) raise(ErrorCode::Generic, "fwrite()");
}

FileI open(const std::filesystem::path& path, openr_t) {
    auto* stream = fopen(path.c_str(), "r");
    if (!stream) raise_errno();
    return FileI(stream);
}

FileO open(const std::filesystem::path& path, openw_t) {
    auto* stream = fopen(path.c_str(), "w");
    if (!stream) raise_errno();
    return FileO(stream);
}

FileIO open(const std::filesystem::path& path, openrw_t) {
    auto* stream = fopen(path.c_str(), "r+");
    if (!stream) raise_errno();
    return FileIO(stream);
}

FileIO open_mem(std::span<std::byte> buf) {
    auto h = fmemopen(buf.data(), buf.size(), "r+");
    if (!h) raise_errno();
    return FileIO(h);
}

std::pair<std::filesystem::path, FileIO> tmpfile() {
    const char* envvar = ::getenv("TMPDIR");
    if (!envvar) envvar = "/tmp";
    auto template_ = std::format("{}/cent.XXXXXX", envvar);
    auto fd = mkstemp(template_.data());
    if (fd < 0) raise_errno();
    auto cleanup = util::Defer([&] { close(fd); });
    FILE* stream = fdopen(fd, "r+");
    if (!stream) raise_errno();
    cleanup.cancel();
    return {std::filesystem::path(std::move(template_)), FileIO(stream)};
}

}  // namespace cent::io
