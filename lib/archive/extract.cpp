#include <archive.h>
#include <archive_entry.h>

#include <cent/archive/extract.hpp>
#include <cent/error.hpp>

namespace cent::archive {
namespace {
int open_callback(::archive* a, void* userdata);
la_ssize_t read_callback(::archive* a, void* userdata, const void** buffer);
int close_callback(::archive* a, void* userdata);
}  // namespace

struct Ctx {
    std::span<const std::byte> buffer{};
    ::archive* ar;
    Ctx() : ar(archive_read_new()) {
        if (!ar) raise(ErrorCode::Generic, "archive_read_new()");
        archive_read_support_filter_all(ar);
        archive_read_support_format_all(ar);
        auto res = archive_read_open(ar, this, &open_callback, &read_callback,
                                     &close_callback);
        if (res != ARCHIVE_OK)
            raise(ErrorCode::Generic, "{}", archive_error_string(ar));
    }

    ~Ctx() {
        if (ar) archive_read_free(ar);
    }
};

namespace {

int open_callback(::archive* a, void* userdata) {
    (void)a;
    (void)userdata;
    return ARCHIVE_OK;
}

la_ssize_t read_callback(::archive* a, void* userdata, const void** buffer) {
    auto buf = static_cast<Ctx*>(userdata)->buffer;
    *buffer = buf.data();
    return buf.size();
}

int close_callback(::archive* a, void* userdata) {
    (void)a;
    (void)userdata;
    return ARCHIVE_OK;
}

}  // namespace

CStr Entry::path() const noexcept { return archive_entry_pathname(m_e); }

Entry& Entry::operator>>(std::span<std::byte>& buf) {
    auto read = archive_read_data(m_a, buf.data(), buf.size());
    switch (read) {
        case ARCHIVE_FATAL:
        case ARCHIVE_WARN:
        case ARCHIVE_RETRY:
            raise(ErrorCode::Generic, "{}", archive_error_string(m_a));
    }
    buf = buf.subspan(0, read);
    return *this;
}
struct Extractor::Impl : Ctx {
    Entry entry{};
};

Extractor::Extractor() : m_impl(new Impl{}) {}

Extractor::~Extractor() = default;

Entry* Extractor::operator()(std::span<const std::byte> chunk) {
    archive_entry* entry{};
    auto res = archive_read_next_header(m_impl->ar, &entry);
    switch (res) {
        case ARCHIVE_OK: {
            m_impl->entry = Entry{m_impl->ar, entry};
            return &m_impl->entry;
        }
        case ARCHIVE_EOF: return nullptr;
        default: break;
    }
    raise(ErrorCode::Generic, "{}", archive_error_string(m_impl->ar));
}

}  // namespace cent::archive
