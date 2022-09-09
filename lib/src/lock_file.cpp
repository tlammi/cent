#include "cent/lock_file.hpp"

namespace cent {

LockFile::LockFile() {}
LockFile::LockFile(FileSystemApi* fs, const stdfs::path& path)
    : m_fs{fs}, m_handle{fs->lock_file(path)} {}
LockFile::~LockFile() {
    if (m_fs) m_fs->unlock_file(m_handle);
}

}  // namespace cent