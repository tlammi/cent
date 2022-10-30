/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <filesystem>

namespace cent::fs {

namespace stdfs = std::filesystem;

class LockFile {
 public:
    LockFile();
    LockFile(const stdfs::path& path);
    LockFile(const LockFile&) = delete;
    LockFile& operator=(const LockFile&) = delete;
    LockFile(LockFile&& rhs);
    LockFile& operator=(LockFile&& rhs);
    ~LockFile();

 private:
    int m_handle{0};
};
}  // namespace cent::fs