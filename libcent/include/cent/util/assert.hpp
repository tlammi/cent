#pragma once

#include <cent/util/raise.hpp>

#if defined(NDEBUG)
#define CENT_ASSERT(condition) ((void)0)
#else
#define CENT_ASSERT(condition)                                 \
    do {                                                       \
        if (!(condition))                                      \
            ::cent::panic("assert: ", __FILE__, "[", __LINE__, \
                          "]: ", #condition);                  \
    } while (0)
#endif
