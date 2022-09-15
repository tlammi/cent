#pragma once

#include "cent/drv/file_system.hpp"

namespace cent {

class LockFile {
 public:
    LockFile();
    LockFile(drv::FileSystem* fs, const stdfs::path& path);
    LockFile(const LockFile&) = delete;
    LockFile& operator=(const LockFile&) = delete;
    LockFile(LockFile&&) = delete;
    LockFile& operator=(LockFile&&) = delete;
    ~LockFile();

 private:
    drv::FileSystem* m_fs{nullptr};
    int m_handle{0};
};
}  // namespace cent