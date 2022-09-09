#pragma once

#include "cent/file_system_api.hpp"

namespace cent {

class LockFile {
 public:
    LockFile();
    LockFile(FileSystemApi* fs, const stdfs::path& path);
    LockFile(const LockFile&) = delete;
    LockFile& operator=(const LockFile&) = delete;
    LockFile(LockFile&&) = delete;
    LockFile& operator=(LockFile&&) = delete;
    ~LockFile();

 private:
    FileSystemApi* m_fs{nullptr};
    int m_handle{0};
};
}  // namespace cent