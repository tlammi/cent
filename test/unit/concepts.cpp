#include <cent/concepts.hpp>

struct Foo {
    constexpr void operator()() const noexcept {}
};

static_assert(cent::concepts::any_callable<void()>);
static_assert(cent::concepts::any_callable<void (*)()>);
static_assert(cent::concepts::any_callable<Foo>);

