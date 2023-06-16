#include <fmt/printf.h>

#include <cent/core.hpp>
#include <cstdlib>

// NOLINTBEGIN
int main(int argc, char** argv) {
    if (argc != 2) {
        fmt::print(stderr, "Usage: {} {}\n", argv[0], "<url>");
        return EXIT_FAILURE;
    }
    auto req = cent::http_request();
    auto resp = req->get(argv[1]);
    fmt::print(stderr, "status: {}\n", resp.status_code);
    fmt::print(stderr, "body: {}\n", resp.body);
}

// NOLINTEND
