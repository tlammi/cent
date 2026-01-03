#include "module.hpp"

namespace cent::mods::aux {

class IdleCmd final : public component::AuxCmd {
 public:
    std::string_view name() const noexcept { return "idle"; }

    int run(int argc, char** argv) const noexcept { return 0; }
};

constexpr IdleCmd cmd{};

void init(ModuleCtx& ctx) { ctx.add_aux_command(cmd); }
CENT_MODULE(init);

}  // namespace cent::mods::aux
