#include <gtest/gtest.h>

#include <cent/strg/storage.hpp>

using namespace cent::strg;

template <class... Ts>
void mk_vec_impl(auto& out, uint8_t t, Ts&&... ts) {
    out.push_back(std::byte{t});
    if constexpr (sizeof...(Ts)) mk_vec_impl(out, std::forward<Ts>(ts)...);
}

template <class... Ts>
auto mk_vec(Ts&&... ts) {
    auto v = std::vector<std::byte>();
    v.reserve(sizeof...(ts));
    mk_vec_impl(v, std::forward<Ts>(ts)...);
    return v;
}

TEST(Init, InMemory) {
    auto s = in_memory_storage();
    ASSERT_TRUE(s);
}

TEST(Layers, Init) {
    auto s = in_memory_storage();
    auto l = s->layers();
    (void)l;
}

TEST(Layers, Write) {
    auto s = in_memory_storage();
    auto l = s->layers();
    auto blob = l.write("foo", 100);
    auto v = mk_vec(1, 2, 3, 4);
    blob << v;
}

TEST(Layers, Roundtrip) {
    auto s = in_memory_storage();
    auto l = s->layers();
    auto data = mk_vec(1, 2, 3, 4);
    {
        auto out = l.write("foo", 100);
        out << data;
    }
    auto in = l.read("foo");
    auto res = std::vector<std::byte>(4, std::byte{});
    in >> res;
    ASSERT_EQ(res, data);
}
