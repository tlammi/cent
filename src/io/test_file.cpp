#include "file.hpp"

namespace io = cent::io;

TEST(Ctor, Default) {
    auto f = io::File();
    ASSERT_FALSE(f.handle());
}

TEST(Move, Ctor) {
    auto buf = std::vector<std::byte>();
    auto f = io::open_mem(buf);
    auto f2 = std::move(f);
}

TEST(Move, Assign) {
    auto buf = std::vector<std::byte>();
    auto f = io::open_mem(buf);
    auto f2 = io::open_mem(buf);
    f2 = std::move(f);
}

TEST(Mem, Write) {
    auto buf = std::vector<std::byte>(64, std::byte{});
    {
        auto f = io::open_mem(buf);
        f.write("foo");
    }
    auto view = std::string_view(reinterpret_cast<const char*>(buf.data()), 3);
    ASSERT_EQ(view, "foo");
}

TEST(Mem, Read) {
    auto buf = std::string("foobar");
    auto out = std::string(100, '\0');
    {
        auto f = io::open_mem(buf);
        auto count = f.read(out);
        out.resize(count);
    }
    ASSERT_EQ(out, "foobar");
}

TEST(Mem, ReadAll) {
    auto buf = std::string("barfoo");
    auto out = std::string();
    {
        auto f = io::open_mem(buf);
        f >> out;
    }
    ASSERT_EQ(out, "barfoo");
}

TEST(TmpFile, WriteRead) {
    auto input = std::string("foobar");
    auto output = std::string();
    std::filesystem::path path{};
    {
        io::FileIO f{};
        std::tie(path, f) = io::tmpfile();
        f.write(input);
    }
    {
        auto f = io::open(path, io::openr);
        f.seek(0, io::SeekOrigin::Set);
        f >> output;
    }
    ASSERT_EQ(input, output);
}
