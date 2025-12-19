#include "login.hpp"

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

#include "error.hpp"

namespace cent::os {

namespace {

std::string buffer() {
    // Just copied from manpages, no idea if could be something better
    static constexpr auto fallback = 16384;
    auto bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1) bufsize = fallback;
    return std::string(bufsize, '\0');
}

}  // namespace

std::string getlogin(uid_t uid) {
    struct passwd pw{};
    struct passwd* result{};
    auto buf = buffer();
    auto res = getpwuid_r(uid, &pw, buf.data(), buf.size(), &result);
    if (!result) raise_errno(res);
    return std::string(pw.pw_name);
}

std::string getlogin() { return getlogin(geteuid()); }

uid_t getuid(const char* name) {
    struct passwd pw{};
    struct passwd* result{};
    auto buf = buffer();
    auto res = getpwnam_r(name, &pw, buf.data(), buf.size(), &result);
    if (!result) raise_errno(res);
    return pw.pw_uid;
}

}  // namespace cent::os
