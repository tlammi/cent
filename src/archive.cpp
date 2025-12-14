#include "archive.hpp"

#include <archive.h>
#include <archive_entry.h>

#include "error.hpp"
#include "util/defer.hpp"

namespace cent::archive {
namespace {

extern "C" int open_callback(::archive*, void*) { return ARCHIVE_OK; }

extern "C" int close_callback(::archive* ar, void* data) noexcept {
    auto* ptr = static_cast<detail::RangeReader*>(data);
    delete ptr;
    return ARCHIVE_OK;
}

// TODO: Catch exceptions
extern "C" la_ssize_t read_callback(::archive* ar, void* data,
                                    const void** buffer) noexcept {
    auto* ptr = static_cast<detail::RangeReader*>(data);
    auto span = ptr->next();
    *buffer = span.data();
    return span.size();
}

::archive* open_from_buf(std::span<const std::byte> buf) {
    ::archive* ar = archive_read_new();
    if (!ar) raise(ErrorCode::Generic, "archive_read_new()");
    archive_read_support_filter_all(ar);
    archive_read_support_format_all(ar);
    auto res = archive_read_open_memory(ar, buf.data(), buf.size());
    if (res == ARCHIVE_OK) return ar;
    // TODO: leaks
    raise(ErrorCode::Generic, "{}", archive_error_string(ar));
}

::archive* open_from_reader(std::unique_ptr<detail::RangeReader> reader) {
    ::archive* ar = archive_read_new();
    if (!ar) raise(ErrorCode::Generic, "archive_read_new()");
    auto cleanup = util::Defer([&] { archive_read_free(ar); });
    archive_read_support_filter_all(ar);
    archive_read_support_format_all(ar);
    auto res = archive_read_open(ar, reader.release(), open_callback,
                                 read_callback, close_callback);

    if (res != ARCHIVE_OK)
        raise(ErrorCode::Generic, "{}", archive_error_string(ar));
    cleanup.cancel();
    return ar;
}

}  // namespace

CStr Entry::path() const noexcept { return archive_entry_pathname(m_e); }
size_t Entry::size() const noexcept { return archive_entry_size(m_e); }

Entry& Entry::operator>>(io::FileO& f) {
    auto res = archive_read_data_into_fd(m_a, f.fd());
    if (res != ARCHIVE_OK)
        raise(ErrorCode::Generic, "{}", archive_error_string(m_a));
    return *this;
}

size_t Entry::read(std::span<std::byte> buf) {
    auto count = archive_read_data(m_a, buf.data(), buf.size());
    switch (count) {
        case ARCHIVE_FATAL:
        case ARCHIVE_WARN:
        case ARCHIVE_RETRY:
            raise(ErrorCode::Generic, "{}", archive_error_string(m_a));
    }
    return count;
}

Archive::Archive(std::span<const std::byte> buf)
    : m_entry(open_from_buf(buf), nullptr) {}

Archive::~Archive() {
    if (m_entry.m_a) archive_read_free(m_entry.m_a);
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
Archive::Archive(std::unique_ptr<detail::RangeReader> reader)
    : m_entry(open_from_reader(std::move(reader)), nullptr) {}

void extract_to(Archive& ar, const std::filesystem::path& p) {
    namespace fs = std::filesystem;
    fs::create_directories(p);
    for (auto& entry : ar) {
        auto dst = p / entry.path().c_str();
        // TODO: validate that the dst does not escape p
        fs::create_directories(dst.parent_path());
        auto f = io::open(dst, io::openw);
        entry >> f;
    }
}
}  // namespace cent::archive
