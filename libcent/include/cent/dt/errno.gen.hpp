#pragma once
// GENERATED FILE. DO NOT EDIT!
#include <cent/str/static_str.hpp>
#include <cerrno>

namespace cent {
enum class Errno {
    Ok = 0,
    Eperm = 1,
    Enoent = 2,
    Esrch = 3,
    Eintr = 4,
    Eio = 5,
    Enxio = 6,
    E2Big = 7,
    Enoexec = 8,
    Ebadf = 9,
    Echild = 10,
    Eagain = 11,
    Enomem = 12,
    Eacces = 13,
    Efault = 14,
    Enotblk = 15,
    Ebusy = 16,
    Eexist = 17,
    Exdev = 18,
    Enodev = 19,
    Enotdir = 20,
    Eisdir = 21,
    Einval = 22,
    Enfile = 23,
    Emfile = 24,
    Enotty = 25,
    Etxtbsy = 26,
    Efbig = 27,
    Enospc = 28,
    Espipe = 29,
    Erofs = 30,
    Emlink = 31,
    Epipe = 32,
    Edom = 33,
    Erange = 34,
    Edeadlock = 35,
    Enametoolong = 36,
    Enolck = 37,
    Enosys = 38,
    Enotempty = 39,
    Eloop = 40,
    Enomsg = 42,
    Eidrm = 43,
    Echrng = 44,
    El2Nsync = 45,
    El3Hlt = 46,
    El3Rst = 47,
    Elnrng = 48,
    Eunatch = 49,
    Enocsi = 50,
    El2Hlt = 51,
    Ebade = 52,
    Ebadr = 53,
    Exfull = 54,
    Enoano = 55,
    Ebadrqc = 56,
    Ebadslt = 57,
    Ebfont = 59,
    Enostr = 60,
    Enodata = 61,
    Etime = 62,
    Enosr = 63,
    Enonet = 64,
    Enopkg = 65,
    Eremote = 66,
    Enolink = 67,
    Eadv = 68,
    Esrmnt = 69,
    Ecomm = 70,
    Eproto = 71,
    Emultihop = 72,
    Edotdot = 73,
    Ebadmsg = 74,
    Eoverflow = 75,
    Enotuniq = 76,
    Ebadfd = 77,
    Eremchg = 78,
    Elibacc = 79,
    Elibbad = 80,
    Elibscn = 81,
    Elibmax = 82,
    Elibexec = 83,
    Eilseq = 84,
    Erestart = 85,
    Estrpipe = 86,
    Eusers = 87,
    Enotsock = 88,
    Edestaddrreq = 89,
    Emsgsize = 90,
    Eprototype = 91,
    Enoprotoopt = 92,
    Eprotonosupport = 93,
    Esocktnosupport = 94,
    Enotsup = 95,
    Epfnosupport = 96,
    Eafnosupport = 97,
    Eaddrinuse = 98,
    Eaddrnotavail = 99,
    Enetdown = 100,
    Enetunreach = 101,
    Enetreset = 102,
    Econnaborted = 103,
    Econnreset = 104,
    Enobufs = 105,
    Eisconn = 106,
    Enotconn = 107,
    Eshutdown = 108,
    Etoomanyrefs = 109,
    Etimedout = 110,
    Econnrefused = 111,
    Ehostdown = 112,
    Ehostunreach = 113,
    Ealready = 114,
    Einprogress = 115,
    Estale = 116,
    Euclean = 117,
    Enotnam = 118,
    Enavail = 119,
    Eisnam = 120,
    Eremoteio = 121,
    Edquot = 122,
    Enomedium = 123,
    Emediumtype = 124,
    Ecanceled = 125,
    Enokey = 126,
    Ekeyexpired = 127,
    Ekeyrevoked = 128,
    Ekeyrejected = 129,
    Eownerdead = 130,
    Enotrecoverable = 131,
    Erfkill = 132,
};

constexpr StaticStr errno_message(Errno e) noexcept {
    switch (e) {
        case Errno::Ok: return "OK";
        case Errno::Eperm: return "Operation not permitted";
        case Errno::Enoent: return "No such file or directory";
        case Errno::Esrch: return "No such process";
        case Errno::Eintr: return "Interrupted system call";
        case Errno::Eio: return "Input/output error";
        case Errno::Enxio: return "No such device or address";
        case Errno::E2Big: return "Argument list too long";
        case Errno::Enoexec: return "Exec format error";
        case Errno::Ebadf: return "Bad file descriptor";
        case Errno::Echild: return "No child processes";
        case Errno::Eagain: return "Resource temporarily unavailable";
        case Errno::Enomem: return "Cannot allocate memory";
        case Errno::Eacces: return "Permission denied";
        case Errno::Efault: return "Bad address";
        case Errno::Enotblk: return "Block device required";
        case Errno::Ebusy: return "Device or resource busy";
        case Errno::Eexist: return "File exists";
        case Errno::Exdev: return "Invalid cross-device link";
        case Errno::Enodev: return "No such device";
        case Errno::Enotdir: return "Not a directory";
        case Errno::Eisdir: return "Is a directory";
        case Errno::Einval: return "Invalid argument";
        case Errno::Enfile: return "Too many open files in system";
        case Errno::Emfile: return "Too many open files";
        case Errno::Enotty: return "Inappropriate ioctl for device";
        case Errno::Etxtbsy: return "Text file busy";
        case Errno::Efbig: return "File too large";
        case Errno::Enospc: return "No space left on device";
        case Errno::Espipe: return "Illegal seek";
        case Errno::Erofs: return "Read-only file system";
        case Errno::Emlink: return "Too many links";
        case Errno::Epipe: return "Broken pipe";
        case Errno::Edom: return "Numerical argument out of domain";
        case Errno::Erange: return "Numerical result out of range";
        case Errno::Edeadlock: return "Resource deadlock avoided";
        case Errno::Enametoolong: return "File name too long";
        case Errno::Enolck: return "No locks available";
        case Errno::Enosys: return "Function not implemented";
        case Errno::Enotempty: return "Directory not empty";
        case Errno::Eloop: return "Too many levels of symbolic links";
        case Errno::Enomsg: return "No message of desired type";
        case Errno::Eidrm: return "Identifier removed";
        case Errno::Echrng: return "Channel number out of range";
        case Errno::El2Nsync: return "Level 2 not synchronized";
        case Errno::El3Hlt: return "Level 3 halted";
        case Errno::El3Rst: return "Level 3 reset";
        case Errno::Elnrng: return "Link number out of range";
        case Errno::Eunatch: return "Protocol driver not attached";
        case Errno::Enocsi: return "No CSI structure available";
        case Errno::El2Hlt: return "Level 2 halted";
        case Errno::Ebade: return "Invalid exchange";
        case Errno::Ebadr: return "Invalid request descriptor";
        case Errno::Exfull: return "Exchange full";
        case Errno::Enoano: return "No anode";
        case Errno::Ebadrqc: return "Invalid request code";
        case Errno::Ebadslt: return "Invalid slot";
        case Errno::Ebfont: return "Bad font file format";
        case Errno::Enostr: return "Device not a stream";
        case Errno::Enodata: return "No data available";
        case Errno::Etime: return "Timer expired";
        case Errno::Enosr: return "Out of streams resources";
        case Errno::Enonet: return "Machine is not on the network";
        case Errno::Enopkg: return "Package not installed";
        case Errno::Eremote: return "Object is remote";
        case Errno::Enolink: return "Link has been severed";
        case Errno::Eadv: return "Advertise error";
        case Errno::Esrmnt: return "Srmount error";
        case Errno::Ecomm: return "Communication error on send";
        case Errno::Eproto: return "Protocol error";
        case Errno::Emultihop: return "Multihop attempted";
        case Errno::Edotdot: return "RFS specific error";
        case Errno::Ebadmsg: return "Bad message";
        case Errno::Eoverflow: return "Value too large for defined data type";
        case Errno::Enotuniq: return "Name not unique on network";
        case Errno::Ebadfd: return "File descriptor in bad state";
        case Errno::Eremchg: return "Remote address changed";
        case Errno::Elibacc: return "Can not access a needed shared library";
        case Errno::Elibbad: return "Accessing a corrupted shared library";
        case Errno::Elibscn: return ".lib section in a.out corrupted";
        case Errno::Elibmax:
            return "Attempting to link in too many shared libraries";
        case Errno::Elibexec: return "Cannot exec a shared library directly";
        case Errno::Eilseq:
            return "Invalid or incomplete multibyte or wide character";
        case Errno::Erestart:
            return "Interrupted system call should be restarted";
        case Errno::Estrpipe: return "Streams pipe error";
        case Errno::Eusers: return "Too many users";
        case Errno::Enotsock: return "Socket operation on non-socket";
        case Errno::Edestaddrreq: return "Destination address required";
        case Errno::Emsgsize: return "Message too long";
        case Errno::Eprototype: return "Protocol wrong type for socket";
        case Errno::Enoprotoopt: return "Protocol not available";
        case Errno::Eprotonosupport: return "Protocol not supported";
        case Errno::Esocktnosupport: return "Socket type not supported";
        case Errno::Enotsup: return "Operation not supported";
        case Errno::Epfnosupport: return "Protocol family not supported";
        case Errno::Eafnosupport:
            return "Address family not supported by protocol";
        case Errno::Eaddrinuse: return "Address already in use";
        case Errno::Eaddrnotavail: return "Cannot assign requested address";
        case Errno::Enetdown: return "Network is down";
        case Errno::Enetunreach: return "Network is unreachable";
        case Errno::Enetreset: return "Network dropped connection on reset";
        case Errno::Econnaborted: return "Software caused connection abort";
        case Errno::Econnreset: return "Connection reset by peer";
        case Errno::Enobufs: return "No buffer space available";
        case Errno::Eisconn: return "Transport endpoint is already connected";
        case Errno::Enotconn: return "Transport endpoint is not connected";
        case Errno::Eshutdown:
            return "Cannot send after transport endpoint shutdown";
        case Errno::Etoomanyrefs: return "Too many references: cannot splice";
        case Errno::Etimedout: return "Connection timed out";
        case Errno::Econnrefused: return "Connection refused";
        case Errno::Ehostdown: return "Host is down";
        case Errno::Ehostunreach: return "No route to host";
        case Errno::Ealready: return "Operation already in progress";
        case Errno::Einprogress: return "Operation now in progress";
        case Errno::Estale: return "Stale file handle";
        case Errno::Euclean: return "Structure needs cleaning";
        case Errno::Enotnam: return "Not a XENIX named type file";
        case Errno::Enavail: return "No XENIX semaphores available";
        case Errno::Eisnam: return "Is a named type file";
        case Errno::Eremoteio: return "Remote I/O error";
        case Errno::Edquot: return "Disk quota exceeded";
        case Errno::Enomedium: return "No medium found";
        case Errno::Emediumtype: return "Wrong medium type";
        case Errno::Ecanceled: return "Operation canceled";
        case Errno::Enokey: return "Required key not available";
        case Errno::Ekeyexpired: return "Key has expired";
        case Errno::Ekeyrevoked: return "Key has been revoked";
        case Errno::Ekeyrejected: return "Key was rejected by service";
        case Errno::Eownerdead: return "Owner died";
        case Errno::Enotrecoverable: return "State not recoverable";
        case Errno::Erfkill: return "Operation not possible due to RF-kill";
        default: return "Unknown error";
    }
}

constexpr Errno from_errno(int e) noexcept { return static_cast<Errno>(e); }
inline Errno from_errno() noexcept { return from_errno(errno); }
}  // namespace cent
