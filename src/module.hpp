#pragma once

#include "drv/driver.hpp"
#include "drv/mount.hpp"

#define CENT_MODULE_SECTION    cent_modules
#define CENT_STRINGIFY_IMPL(s) #s
#define CENT_STRINGIFY(s)      CENT_STRINGIFY_IMPL(s)

#define CENT_MODULE(fn)                \
    ::cent::ModuleHook cent_module_ptr \
        __attribute__((section(CENT_STRINGIFY(CENT_MODULE_SECTION)))) = &fn;

namespace cent {
class ModuleCtx {
 public:
    virtual void add_driver(const drv::Meta<drv::Mount>& meta) = 0;

 protected:
    ~ModuleCtx() = default;
};
using ModuleHook = void (*)(ModuleCtx&);

void init_modules(ModuleCtx& ctx);
}  // namespace cent
