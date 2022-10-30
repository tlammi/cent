/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include "cent/fs.hpp"

namespace cent::fs {

LockFile::LockFile() {}
LockFile::LockFile(const stdfs::path& path) : m_handle{lock_file(path)} {}

LockFile::LockFile(LockFile&& rhs) : m_handle{rhs.m_handle} {
    rhs.m_handle = 0;
}
LockFile& LockFile::operator=(LockFile&& rhs) {
    if (m_handle) { unlock_file(m_handle); }
    m_handle = rhs.m_handle;
    rhs.m_handle = 0;
    return *this;
}

LockFile::~LockFile() {
    if (m_handle) unlock_file(m_handle);
}

}  // namespace cent::fs