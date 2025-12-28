#include <algorithm>

#include "stream.hpp"

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

template <class... Ts>
std::vector<std::byte> mk_buf(Ts&&... ts) {
    return {std::bit_cast<std::byte>(ts)...};
}

TEST(IStream, ReadStr) {
    auto s = MockIStream{mk_buf('a', 'b', 'c')};
    auto str = std::string();
    s >> str;
    ASSERT_EQ(str, "abc");
}

TEST(IStream, ReadStrUnknownSize) {
    auto s = MockIStream(mk_buf('a', 'b', 'c'), io::UNKNOWN_SIZE);
    auto str = std::string();
    s >> str;
    ASSERT_EQ(str, "abc");
}
