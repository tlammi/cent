#pragma once

#include <unistd.h>

#include <filesystem>
#include <limits>
#include <memory>
#include <span>

#include "concepts.hpp"
#include "io/fd.hpp"

namespace cent::io {
namespace detail {

template <class C, class S>
constexpr auto span_cast(S& s) {
    return std::span(reinterpret_cast<C*>(s.data()), s.size());
}
}  // namespace detail

class Atomic {
 public:
    constexpr virtual ~Atomic() noexcept = default;

    virtual void commit() = 0;
};

static constexpr auto UNKNOWN_SIZE = std::numeric_limits<size_t>::max();

class OStream {
 public:
    virtual ~OStream() = default;
    virtual size_t write(std::span<const std::byte> buf) = 0;
    template <anyspanlike S>
    size_t write(const S& buf) {
        return write(detail::span_cast<const std::byte>(buf));
    }
};

template <class T>
OStream& operator<<(OStream& os, T&& t) {
    auto span = std::span(std::forward<T>(t));
    while (!span.empty()) {
        auto count = os.write(span);
        span = span.subspan(count);
    }
    return os;
}

class AtomicOStream : public OStream, public Atomic {};

class OFStream : public OStream {
 public:
    explicit constexpr OFStream(int fd) noexcept : m_fd(fd) {}
    explicit OFStream(const char* path);
    explicit OFStream(const std::filesystem::path& path)
        : OFStream(path.c_str()) {}
    size_t size();
    size_t write(std::span<const std::byte> buf) override;

 private:
    Fd m_fd{};
};

class AtomicOFStream : public AtomicOStream {
 public:
    explicit AtomicOFStream(std::filesystem::path path);

    size_t size();
    size_t write(std::span<const std::byte> buf) override;

    void commit() override;

 private:
    std::filesystem::path m_tgt{};
    Fd m_fd;
};

class AnyOStream : public OStream {
 public:
    explicit AnyOStream(std::unique_ptr<OStream> impl) noexcept
        : m_impl(std::move(impl)) {}

    size_t write(std::span<const std::byte> buf) override {
        return m_impl->write(buf);
    }

 private:
    std::unique_ptr<OStream> m_impl;
};

class IStream {
 public:
    virtual ~IStream() = default;
    virtual size_t size() = 0;
    virtual size_t read(std::span<std::byte> buf) = 0;
    template <character_type C>
    size_t read(std::span<C> buf) {
        return read(detail::span_cast<std::byte>(buf));
    }
};

template <class T>
IStream& operator>>(IStream& is, T& out) {
    out.clear();
    if (auto sz = is.size(); sz != UNKNOWN_SIZE) {
        out.resize(sz);
        auto span = std::span(out.data(), out.size());
        while (!span.empty()) {
            auto count = is.read(span);
            span = span.subspan(count);
        }
    } else {
        static constexpr auto chunk_size = 1024uz;
        auto total_read = 0uz;
        while (true) {
            auto orig_sz = out.size();
            out.resize(orig_sz + chunk_size);
            auto span = std::span(&out[orig_sz], chunk_size);
            while (true) {
                auto count = is.read(span);
                if (!count) {
                    // eof
                    out.resize(total_read);
                    return is;
                }
                total_read += count;
                span = span.subspan(count);
                if (span.empty()) break;
            }
        }
    }
    return is;
}

class IFStream : public IStream {
 public:
    constexpr explicit IFStream(int fd) noexcept : m_fd(fd) {}

    size_t size() override;
    size_t read(std::span<std::byte> buf) override;

 private:
    Fd m_fd;
};

class AnyIStream : public IStream {
 public:
    constexpr explicit AnyIStream(std::unique_ptr<IStream> impl) noexcept
        : m_impl(std::move(impl)) {}
    size_t size() override { return m_impl->size(); }
    size_t read(std::span<std::byte> buf) override { return m_impl->read(buf); }

 private:
    std::unique_ptr<IStream> m_impl;
};
}  // namespace cent::io
