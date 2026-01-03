
#include "module.hpp"

using namespace std::literals;

namespace cent::mods {
namespace {
class OverlayMount : public component::Mount {
 public:
 private:
};

struct Meta final : public component::Meta<component::Mount> {
    std::string_view name() const noexcept override { return "overlay"sv; }

    bool supported(component::ProgramCtxView& ctx) const override {
        return true;
    }

    std::unique_ptr<component::Mount> create() const override {
        return std::make_unique<OverlayMount>();
    }
};

constexpr Meta meta{};

}  // namespace

void init(ModuleCtx& ctx) { ctx.add_driver(meta); }

CENT_MODULE(init);

}  // namespace cent::mods
