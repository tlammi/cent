#include <cent/core.hpp>
#include <iostream>
#include <string_view>

namespace cent::bin {

struct BinOpt {
    std::string_view sflag;
    std::string_view lflag;
    std::string_view help;
    bool set;
};

struct ValOpt {
    std::string_view sflag;
    std::string_view lflag;
    std::string_view help;
    std::string_view val;
};

struct VectOpt {
    std::string_view sflag;
    std::string_view lflag;
    std::string_view help;
    std::vector<std::string_view> vals;
};

using Opt = std::variant<BinOpt, ValOpt, VectOpt>;

int cli_root(int argc, char** argv) {
    static constexpr auto opts =
        core::make_array<Opt>(ValOpt{"-l", "--log-level", "Log level"});
    auto iter = core::cli::ArgIter(argc, argv);
    while (true) {
        auto arg = iter.next();
        if (!arg) return EXIT_FAILURE;
    }
}
}  // namespace cent::bin

int main(int argc, char** argv) {
    cent::core::logs::init([](cent::core::logs::Level lvl, std::string s) {
        std::clog << s << '\n';
    });
    return cent::bin::cli_root(argc, argv);
}
