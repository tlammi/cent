#include "error.hpp"
#include "module.hpp"

namespace cent::mods::strg {
namespace {

class Impl final : public component::Storage {
 public:
    std::string manifest(std::string_view digest) override { return ""; }

    void manifest_set(std::string_view digest,
                      std::string_view value) override {}

    bool manifest_rm(std::string_view digest) override { return true; }

    std::vector<std::string> manifest_ls() override { return {}; }

    std::string config(std::string_view digest) override { return {}; }

    void config_set(std::string_view digest, std::string_view value) override {}

    bool config_rm(std::string_view digest) override { return true; }

    std::vector<std::string> config_ls() override { return {}; }

    io::AnyOStream layer_write(std::string_view digest) override {
        raise(ErrorCode::Internal, "Not implemented");
    }

    io::AnyIStream layer_read(std::string_view digest) override {
        raise(ErrorCode::Internal, "Not implemented");
    }

    bool layer_rm(std::string_view digest) override { return true; }

    std::vector<std::string> layer_ls() override { return {}; }

    void migrate(component::Storage& other) override {}

 private:
};

class Meta final : public component::Meta<component::Storage> {
 public:
    std::string_view name() const noexcept override { return "sqlite"; }
    bool supported(component::ProgramCtxView& ctx) const override {
        return true;
    }

    std::unique_ptr<component::Storage> create() const override {
        return std::make_unique<Impl>();
    }
};

constexpr Meta meta{};

void init(::cent::ModuleCtx& ctx) { ctx.add_component(meta); }
}  // namespace
CENT_MODULE(init);
}  // namespace cent::mods::strg
