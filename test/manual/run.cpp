#include "process/run.hpp"

#include <ranges>
#include <vector>

namespace pr = cent::process;

int main(int argc, char** argv) {
    auto args =
        std::span(argv + 1, argc - 1) |
        std::views::transform([](char* s) { return std::string_view(s); }) |
        std::ranges::to<std::vector>();
    auto res = pr::run(args, {});
}
