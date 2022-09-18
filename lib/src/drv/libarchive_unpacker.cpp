/* SPDX-License-Identifier:  GPL-3.0-or-later */
#include <archive.h>
#include <libarchive/archive_entry.h>

#include "cent/defer.hpp"
#include "cent/drv/unpacker.hpp"
#include "cent/logs.hpp"
#include "cent/raise.hpp"

namespace cent::drv {

#define CALL_AND_CHECK(f, a, ...)                                \
    [&]() {                                                      \
        int res = f(a, ##__VA_ARGS__);                           \
        if (res < ARCHIVE_OK)                                    \
            raise(#f, ": ", archive_error_string(a), ", ", res); \
        return res;                                              \
    }()

namespace {

void copy_data(archive* in, archive* out) {
    while (true) {
        size_t size;
        const void* buf;
        int64_t offset;
        int r =
            CALL_AND_CHECK(archive_read_data_block, in, &buf, &size, &offset);
        if (r == ARCHIVE_EOF) { return; }
        CALL_AND_CHECK(archive_write_data_block, out, buf, size, offset);
    }
}

}  // namespace

class LibarchiveUnpacker final : public Unpacker {
 public:
    void unpack(const stdfs::path& src, const stdfs::path& dst) override {
        archive* a = archive_read_new();
        if (!a) raise("archive_read_new()");
        Defer a_free{[&] { archive_read_free(a); }};
        CALL_AND_CHECK(archive_read_support_format_tar, a);
        CALL_AND_CHECK(archive_read_support_filter_gzip, a);
        CALL_AND_CHECK(archive_read_open_filename, a, src.c_str(), 10240);
        Defer a_close{[&] { archive_read_close(a); }};
        archive* out = archive_write_disk_new();
        if (!out) raise("archive_write_disk_new()");
        Defer out_defer{[&] {
            archive_write_close(out);
            archive_write_free(out);
        }};

        static constexpr int secure_flags =
            /*  ARCHIVE_EXTRACT_SECURE_NODOTDOT | */
            ARCHIVE_EXTRACT_SECURE_SYMLINKS;  // Do not extract when resulting
                                              // symlink would relocate the
                                              // file

        static constexpr int attribute_flags =
            ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS |
            ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_XATTR |
            ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_OWNER;
        static constexpr int optimization_flags =
            // ARCHIVE_EXTRACT_UNLINK |  // faster extract, might break existing
            //  hardlinks
            ARCHIVE_EXTRACT_SPARSE;  // Try to keep sparse files sparse

        static constexpr int flags =
            secure_flags | attribute_flags | optimization_flags;

        CALL_AND_CHECK(archive_write_disk_set_standard_lookup, out);
        CALL_AND_CHECK(archive_write_disk_set_options, out, flags);

        stdfs::path orig_path = stdfs::current_path();
        Defer restore_path{[&] {
            logs::debug("Restoring working directory");
            stdfs::current_path(orig_path);
        }};

        logs::debug("Extracting from ", src, " to ", dst);
        logs::debug("Changing current directory to ", dst);
        stdfs::current_path(dst);

        while (true) {
            archive_entry* e{};
            int r = CALL_AND_CHECK(archive_read_next_header, a, &e);
            if (r == ARCHIVE_EOF) break;
            stdfs::path p{archive_entry_pathname(e)};
            logs::trace("Extracting ", p, " to ", dst / p);
            archive_entry_set_pathname(e, p.c_str());
            CALL_AND_CHECK(archive_write_header, out, e);
            if (archive_entry_size(e) > 0) { copy_data(a, out); }
            CALL_AND_CHECK(archive_write_finish_entry, out);
        }
        stdfs::current_path(orig_path);
    }
};
std::unique_ptr<Unpacker> default_unpacker() {
    return std::make_unique<LibarchiveUnpacker>();
}
}  // namespace cent::drv