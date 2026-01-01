#include "module.hpp"

#define JOIN2(a, b) a##b
#define JOIN(a, b)  JOIN2(a, b)

#define MODULES_START JOIN(__start_, CENT_MODULE_SECTION)
#define MODULES_STOP  JOIN(__stop_, CENT_MODULE_SECTION)

extern "C" {
extern cent::ModuleHook MODULES_START[];
extern cent::ModuleHook MODULES_STOP[];
}
namespace cent {
void init_modules(ModuleCtx& ctx) {
    for (auto* ptr = MODULES_START; ptr != MODULES_STOP; ++ptr) (*ptr)(ctx);
}
}  // namespace cent
