#pragma once

#include <type_traits>
#include <utility>

namespace cent {

/**
 * Handle holding a callable which is invoked when the handle expires
 *
 * This can be used e.g. if resources need to be freed before exiting the scope.
 * */
template <class F>
class Defer {
 public:
    static_assert(std::is_nothrow_invocable_v<F>,
                  "Called in dtor. Needs to be nothrow invocable");

    /**
     * Create a new handle
     *
     * \param f Callable to invoke in destructor
     * */
    template <class F2>
    constexpr explicit Defer(F2&& f) : m_f{std::forward<F2>(f)} {}

    Defer(const Defer&) = delete;
    Defer& operator=(const Defer&) = delete;
    Defer(Defer&&) = delete;
    Defer& operator=(Defer&&) = delete;

    ~Defer() {
        if (!m_cancelled) m_f();
    }

    /**
     * Set cancel state
     *
     * If the object is "cancelled" the callback specified in the ctor is never
     * called.
     * */
    void cancel(bool val = true) { m_cancelled = val; }

 private:
    F m_f;
    bool m_cancelled{false};
};

template <class F>
Defer(F) -> Defer<F>;

}  // namespace cent
