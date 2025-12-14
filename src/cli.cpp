#include "cli.hpp"

#include <grove.hpp>

using grove::Branch;
using grove::Leaf;
using grove::PosArg;
using grove::SubCmd;

template <>
struct grove::meta_for<cent::bin::Pull> {
    static constexpr auto meta() {
        return Leaf{
            .args =
                std::tuple{
                    PosArg{&cent::bin::Pull::image, "image", "Image to pull"},
                },
            .help = "pull an image",
        };
    }
};

template <>
struct grove::meta_for<cent::bin::Cli> {
    static constexpr auto meta() {
        return Branch{
            .target = &cent::bin::Cli::cmd,
            .args =
                std::tuple{
                    SubCmd{
                        .name = "pull",
                        .type = grove::tag<cent::bin::Pull>,
                    },
                },
            .help = "Container Engine",

        };
    }
};

namespace cent::bin {

Cli parse_cli(int argc, char** argv) {
    return grove::parse_default<Cli>(argc, argv);
}
}  // namespace cent::bin
