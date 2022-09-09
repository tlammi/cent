#include "cent/storage.hpp"

#include "cent/lock_file.hpp"

namespace cent {
namespace {
stdfs::path layerdir(const stdfs::path& root) {
    stdfs::path out{root};
    out /= "layers";
    return out;
}

stdfs::path configdir(const stdfs::path& root) {
    stdfs::path out{root};
    out /= "configs";
    return out;
}

stdfs::path lockfile(const stdfs::path& root) {
    stdfs::path out{root};
    out /= "cent.lock";
    return out;
}

stdfs::path layerpath(const stdfs::path& root, DigestView digest) {
    auto path = layerdir(root);
    path /= digest.str();
    return path;
}

stdfs::path configpath(const stdfs::path& root, DigestView digest) {
    auto path = configdir(root);
    path /= digest.str();
    return path;
}
}  // namespace

Storage::Storage(FileSystemApi* fs, const stdfs::path& root)
    : m_fs{fs}, m_root{root} {
    LockFile lk{m_fs, lockfile(m_root)};
    m_fs->mkdir(layerdir(m_root), true);
    m_fs->mkdir(configdir(m_root), true);
}

bool Storage::layer_exists(DigestView digest) const {
    auto path = layerpath(m_root, digest);
    LockFile lk{m_fs, lockfile(m_root)};
    return m_fs->exists(path);
}

bool Storage::config_exists(DigestView digest) const {
    auto path = configpath(m_root, digest);
    LockFile lk{m_fs, lockfile(m_root)};
    return m_fs->exists(path);
}

std::unique_ptr<std::iostream> Storage::write_layer(DigestView digest) {
    auto path = layerpath(m_root, digest);
    LockFile lk{m_fs, lockfile(m_root)};
    return m_fs->open_file(path, std::ios_base::out | std::ios_base::binary);
}

std::unique_ptr<std::iostream> Storage::read_layer(DigestView digest) {
    auto path = layerpath(m_root, digest);
    LockFile lk{m_fs, lockfile(m_root)};
    return m_fs->open_file(path, std::ios_base::in | std::ios_base::binary);
}

std::unique_ptr<std::iostream> Storage::write_config(DigestView digest) {
    auto path = configpath(m_root, digest);
    LockFile lk{m_fs, lockfile(m_root)};
    return m_fs->open_file(path, std::ios_base::out | std::ios_base::binary);
}

std::unique_ptr<std::iostream> Storage::read_config(DigestView digest) {
    auto path = configpath(m_root, digest);
    LockFile lk{m_fs, lockfile(m_root)};
    return m_fs->open_file(path, std::ios_base::in | std::ios_base::binary);
}

}  // namespace cent