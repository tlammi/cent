
#include "module.hpp"

using namespace std::literals;

namespace cent::mods {
namespace {
class OverlayMount : public drv::Mount {
 public:
 private:
};

struct Meta final : public drv::Meta<drv::Mount> {
    std::string_view name() const noexcept override { return "overlay"sv; }

    bool supported(drv::ProgramCtxView& ctx) const override { return true; }

    std::unique_ptr<drv::Mount> create() const override {
        return std::make_unique<OverlayMount>();
    }
};

constexpr Meta meta{};

}  // namespace

void init(ModuleCtx& ctx) { ctx.add_driver(meta); }

// CENT_MODULE(init);

::cent::ModuleHook cent_module_ptr
    __attribute__((section("cent_modules"), used)) = &init;

}  // namespace cent::mods
