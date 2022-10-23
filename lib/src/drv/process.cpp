#include "cent/drv/process.hpp"

#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>

#include "cent/raise.hpp"

namespace cent::drv {
namespace {

template <class T, class... Ps>
auto check_errno(T&& t, Ps&&... ps) {
    int res = t(std::forward<Ps>(ps)...);
    if (res < 0) throw std::runtime_error(strerror(errno));
    return res;
}

struct PipeBase {
    int fd{0};
    PipeBase() {}
    explicit PipeBase(int fd) : fd{fd} {}

    PipeBase(const PipeBase&) = delete;
    PipeBase& operator=(const PipeBase&) = delete;

    PipeBase(PipeBase&& other) noexcept : fd{other.fd} { other.fd = 0; }
    PipeBase& operator=(PipeBase& other) noexcept {
        if (fd) { ::close(fd); }
        fd = other.fd;
        other.fd = 0;
        return *this;
    }
    ~PipeBase() { close(); }

    void close() {
        if (fd) ::close(fd);
        fd = 0;
    }

    void duplicate(int newfd) { check_errno(dup2, fd, newfd); }
};

struct ReadPipe : PipeBase {
    using PipeBase::PipeBase;
    std::string read() {
        static constexpr auto size = 1024;
        std::stringstream ss{};
        char buf[size];
        while (true) {
            auto count = ::read(fd, buf, size);
            if (!count) break;
            buf[count] = '\0';
            ss << buf;
        }
        return ss.str();
    }
};
struct WritePipe : PipeBase {
    using PipeBase::PipeBase;
    void write(std::string_view str) {
        while (true) {
            auto count = check_errno(::write, fd, str.data(), str.size());
            str.remove_prefix(count);
            if (str.empty()) break;
        }
    }
};

struct Pipe {
    ReadPipe read{};
    WritePipe write{};

    void close() {
        read.close();
        write.close();
    }
};

Pipe make_pipe() {
    int handles[2];
    check_errno(pipe, handles);
    return Pipe{ReadPipe{handles[0]}, WritePipe{handles[1]}};
}

pid_t do_fork() { return check_errno(fork); }

// Nullptr terminated vector for passing arguments
using ArgVect = std::vector<char*>;

char* alloc_str(std::string_view str) {
    char* buf = new char[str.size() + 1];
    for (size_t i = 0; i < str.size(); ++i) { buf[i] = str[i]; }
    buf[str.size()] = '\0';
    return buf;
}

ArgVect make_arg_vect(const StrVect& v) {
    ArgVect out{};
    out.reserve(v.size() + 1);
    for (auto s : v) { out.push_back(alloc_str(s)); }
    out.push_back(nullptr);
    return out;
}

}  // namespace

Process::Process() {}

Process& Process::args(const StrVect& a) {
    m_args = &a;
    return *this;
}

Process& Process::env(const StrVect& e) {
    m_env = &e;
    return *this;
}

Process& Process::out(std::string& o) {
    m_out = &o;
    return *this;
}
Process& Process::err(std::string& e) {
    m_err = &e;
    return *this;
}

Process& Process::in(const std::string* i) {
    m_in = i;
    return *this;
}

int Process::run() {
    auto do_make_pipe = [&](bool enable) -> std::unique_ptr<Pipe> {
        if (enable) return std::make_unique<Pipe>(make_pipe());
        return nullptr;
    };
    auto out = do_make_pipe(m_out != nullptr);
    auto err = do_make_pipe(m_err != nullptr);
    auto in = do_make_pipe(m_in != nullptr);

    auto pid = do_fork();
    int status{0};
    if (!pid) {
        if (out) {
            out->write.duplicate(STDOUT_FILENO);
            out->close();
        }
        if (err) {
            err->write.duplicate(STDERR_FILENO);
            err->close();
        }
        if (in) {
            in->read.duplicate(STDIN_FILENO);
            in->close();
        }

        auto avect = make_arg_vect(*m_args);
        auto evect = make_arg_vect(*m_env);
        check_errno(execve, avect[0], avect.data(), evect.data());
    } else {
        if (out) out->write.close();
        if (err) err->write.close();
        if (in) {
            in->read.close();
            in->write.write(*m_in);
            in->write.close();
        }
        int status{0};
        wait(&status);
        if (out) *m_out = out->read.read();
        if (err) *m_err = err->read.read();
        return WEXITSTATUS(status);
    }
    raise("Error in PID handling");
}

}  // namespace cent::drv