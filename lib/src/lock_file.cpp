/* SPDX-License-Identifier:  GPL-3.0-or-later */
#include "cent/lock_file.hpp"

namespace cent {

LockFile::LockFile() {}
LockFile::LockFile(drv::FileSystem* fs, const stdfs::path& path)
    : m_fs{fs}, m_handle{fs->lock_file(path)} {}

LockFile::LockFile(LockFile&& rhs) : m_fs{rhs.m_fs}, m_handle{rhs.m_handle} {
    rhs.m_fs = nullptr;
    rhs.m_handle = 0;
}
LockFile& LockFile::operator=(LockFile&& rhs) {
    if (m_fs) { m_fs->unlock_file(m_handle); }
    m_fs = rhs.m_fs;
    m_handle = rhs.m_handle;
    rhs.m_fs = nullptr;
    rhs.m_handle = 0;
    return *this;
}

LockFile::~LockFile() {
    if (m_fs) m_fs->unlock_file(m_handle);
}

}  // namespace cent