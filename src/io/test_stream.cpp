#include <algorithm>

#include "stream.hpp"

using namespace std::literals;

namespace io = cent::io;
struct MockIStream final : public io::IStream {
    explicit MockIStream(std::vector<std::byte> buf) : buf(std::move(buf)) {}
    explicit MockIStream(std::vector<std::byte> buf, size_t buf_size)
        : buf(std::move(buf)), buf_size(buf_size) {}

    std::vector<std::byte> buf{};
    size_t buf_size = buf.size();
    size_t offset = 0;

    size_t size() override { return buf_size; }

    size_t read(std::span<std::byte> buf) override {
        auto sz = std::min(buf.size(), this->buf.size() - offset);
        if (!sz) return 0;
        auto* ptr = &this->buf[offset];
        auto in = std::span(ptr, sz);
        std::copy(in.begin(), in.end(), buf.begin());
        offset += sz;
        return in.size();
    }
};

std::vector<std::byte> mk_buf(std::string_view s) {
    return s | std::views::transform([](char c) {
               return std::bit_cast<std::byte>(c);
           }) |
           std::ranges::to<std::vector>();
}

TEST(IStream, ReadStr) {
    auto in = "abc"sv;
    auto s = MockIStream{mk_buf(in)};
    auto out = std::string();
    s >> out;
    ASSERT_EQ(out, in);
}

TEST(IStream, ReadStrUnknownSize) {
    auto in = "abc"sv;
    auto s = MockIStream(mk_buf(in), io::UNKNOWN_SIZE);
    auto out = std::string();
    s >> out;
    ASSERT_EQ(out, in);
}

struct MockOStream final : public io::OStream {
    std::string buf{};
    size_t max_write{std::numeric_limits<size_t>::max()};

    size_t write(std::span<const std::byte> buf) override {
        if (buf.size() > max_write) buf = buf.subspan(0, max_write);
        auto orig_size = this->buf.size();
        this->buf.append_range(buf | std::views::transform([](std::byte b) {
                                   return std::bit_cast<char>(b);
                               }));
        return this->buf.size() - orig_size;
    }
};

TEST(OStream, WriteAll) {
    auto s = MockOStream();
    auto in = "abc"sv;
    s << in;
    ASSERT_EQ(s.buf, in);
}

TEST(OStream, WriteInParts) {
    auto s = MockOStream();
    s.max_write = 2;
    auto in = "abcdefg"sv;
    s << in;
    ASSERT_EQ(s.buf, in);
}
