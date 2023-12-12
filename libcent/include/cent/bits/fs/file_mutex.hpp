#pragma once

#include <fcntl.h>

#include <cent/util.hpp>

namespace cent {

class FileMutex {
 public:
    constexpr explicit FileMutex(int fd) : m_fd(fd) {}

    void lock() {
        struct flock args {
            .l_type = F_WRLCK, .l_whence = SEEK_SET, .l_start = 0, .l_len = 0,
        };

        int res = fcntl(m_fd, F_SETLKW, &args);  // NOLINT
        CENT_CHECK(res == 0);
    }

    bool try_lock() {
        struct flock args {
            .l_type = F_WRLCK, .l_whence = SEEK_SET, .l_start = 0, .l_len = 0,
        };

        int res = fcntl(m_fd, F_SETLK, &args);  // NOLINT
        if (res == 0) return true;
        CENT_CHECK(errno == EACCES || errno == EAGAIN);
        return false;
    }

    void unlock() {
        struct flock args {
            .l_type = F_UNLCK, .l_whence = SEEK_SET, .l_start = 0, .l_len = 0,
        };
        int res = fcntl(m_fd, F_SETLKW, &args);  // NOLINT
        CENT_CHECK(res == 0);
    }

    void lock_shared() {
        struct flock args {
            .l_type = F_RDLCK, .l_whence = SEEK_SET, .l_start = 0, .l_len = 0,
        };
        int res = fcntl(m_fd, F_SETLKW, &args);  // NOLINT
        CENT_CHECK(res == 0);
    }

    bool try_lock_shared() {
        struct flock args {
            .l_type = F_RDLCK, .l_whence = SEEK_SET, .l_start = 0, .l_len = 0,
        };
        int res = fcntl(m_fd, F_SETLK, &args);  // NOLINT
        if (res == 0) return true;
        CENT_CHECK(errno == EACCES || errno == EAGAIN);
        return false;
    }

    void unlock_shared() { unlock(); }

 private:
    int m_fd{0};
};
}  // namespace cent
