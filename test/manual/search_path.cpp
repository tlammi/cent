#include "process/search_path.hpp"

#include <print>

int main(int argc, char** argv) {
    if (argc != 2) throw std::runtime_error("usage: search_path <cmd>");
    std::println("{}", cent::process::search_path(argv[1]).native());
}
