/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include "cent/lock_file.hpp"

#include "cent/drv/file_system_impl.hpp"

namespace cent {

LockFile::LockFile() {}
LockFile::LockFile(const stdfs::path& path)
    : m_handle{drv::fs().lock_file(path)} {}

LockFile::LockFile(LockFile&& rhs) : m_handle{rhs.m_handle} {
    rhs.m_handle = 0;
}
LockFile& LockFile::operator=(LockFile&& rhs) {
    if (m_handle) { drv::fs().unlock_file(m_handle); }
    m_handle = rhs.m_handle;
    rhs.m_handle = 0;
    return *this;
}

LockFile::~LockFile() {
    if (m_handle) drv::fs().unlock_file(m_handle);
}

}  // namespace cent