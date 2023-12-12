
#include <cent/net.hpp>
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: prog <url>\n";
        return EXIT_FAILURE;
    }

    cent::net::Session sess{};
    sess.on_header([&](std::string_view field, std::string_view val) -> bool {
        std::cerr << "header: " << field << ": " << val << '\n';
        return true;
    });

    sess.on_write([&](std::string_view data) -> bool {
        std::cout << "body: " << data << '\n';
        return true;
    });

    sess.url(argv[1]);
    sess.get();
}
