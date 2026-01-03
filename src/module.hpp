#pragma once

#include "component/auxcmd.hpp"
#include "component/meta.hpp"
#include "component/mount.hpp"
#include "component/storage.hpp"

#define CENT_MODULE_SECTION    cent_modules
#define CENT_STRINGIFY_IMPL(s) #s
#define CENT_STRINGIFY(s)      CENT_STRINGIFY_IMPL(s)

#define CENT_MODULE(fn)                \
    ::cent::ModuleHook cent_module_ptr \
        __attribute__((section(CENT_STRINGIFY(CENT_MODULE_SECTION)))) = &fn;

namespace cent {
class ModuleCtx {
 public:
    virtual void add_component(const component::Meta<component::Mount>& meta) {}
    virtual void add_component(
        const component::Meta<component::Storage>& meta) {}

    virtual void add_aux_command(const component::AuxCmd& cmd) {}

 protected:
    ~ModuleCtx() = default;
};
using ModuleHook = void (*)(ModuleCtx&);

void init_modules(ModuleCtx& ctx);
}  // namespace cent
