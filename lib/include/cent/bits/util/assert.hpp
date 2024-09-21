#pragma once

#include <exception>

#if defined(NDEBUG)
#define CENT_ASSERT(...) (0(void))
#else
#define CENT_ASSERT(condition)                                 \
    do {                                                       \
        if (!(condition)) [[unlikely]] { ::std::terminate(); } \
    } while (0)
#endif
