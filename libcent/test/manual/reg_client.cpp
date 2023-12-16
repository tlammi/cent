
#include <cent/reg.hpp>
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <url>\n";
        return EXIT_FAILURE;
    }
    auto client = cent::reg::client();
    std::cout << client->manifest_list(argv[1]) << '\n';
}
