#include "archive.hpp"
#include "io/file.hpp"

std::generator<std::span<const std::byte>> reader(cent::io::FileI f) {
    std::vector<std::byte> buf(1024, std::byte{});
    while (true) {
        auto count = f.read(buf);
        if (count == 0) co_return;
        co_yield std::span(buf.data(), count);
    }
}

int main(int argc, char** argv) {
    if (argc != 3) throw std::runtime_error("extractor <archive> <directory>");
    auto f = cent::io::open(argv[1], cent::io::openr);
    auto r = reader(std::move(f));
    auto arc = cent::archive::Archive(r);
    cent::archive::extract_to(arc, argv[2]);
}
