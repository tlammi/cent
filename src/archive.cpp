#include "archive.hpp"

#include <archive.h>
#include <archive_entry.h>

#include "error.hpp"

namespace cent::archive {
namespace {
::archive* open_from_buf(std::span<const std::byte> buf) {
    ::archive* ar = archive_read_new();
    if (!ar) raise(ErrorCode::Generic, "archive_read_new()");
    archive_read_support_filter_all(ar);
    archive_read_support_format_all(ar);
    auto res = archive_read_open_memory(ar, buf.data(), buf.size());
    if (res == ARCHIVE_OK) return ar;
    raise(ErrorCode::Generic, "{}", archive_error_string(ar));
}
}  // namespace

CStr Entry::path() const noexcept { return archive_entry_pathname(m_e); }
Archive::Archive(std::span<const std::byte> buf)
    : m_entry(open_from_buf(buf), nullptr) {}

Archive::~Archive() {
    if (m_entry.m_a) archive_read_close(m_entry.m_a);
}

CStr Archive::compression_name() const noexcept {
    assert(m_entry.m_a);
    const char* ptr = archive_filter_name(m_entry.m_a, 0);
    assert(ptr);
    return ptr;
}

auto Archive::iterator::operator++() -> iterator& {
    auto res = archive_read_next_header(m_e->m_a, &m_e->m_e);
    switch (res) {
        case ARCHIVE_OK: break;
        case ARCHIVE_EOF: m_e->m_e = nullptr; break;
        default:
            raise(ErrorCode::Generic, "{}", archive_error_string(m_e->m_a));
    }
    return *this;
}

bool Archive::iterator::operator==(std::default_sentinel_t) const noexcept {
    return !m_e->m_e;
}

}  // namespace cent::archive
