#pragma once

#include <cent/app/arg_iter.hpp>

namespace cent::app {

template <class CharT>
class Args {
 public:
    constexpr Args() = default;
    constexpr Args(int argc, CharT** argv)
        : m_argc{static_cast<size_t>(argc)}, m_argv{argv} {}
    constexpr Args(size_t argc, CharT** argv) : m_argc{argc}, m_argv{argv} {}

    constexpr ArgIter<CharT> begin() { return {m_argc, m_argv}; }
    constexpr ArgIter<CharT> end() { return {}; }

 private:
    size_t m_argc{};
    CharT** m_argv{};
};

template <class CharT>
constexpr Args<CharT> args(int argc, CharT** argv) {
    return {argc, argv};
}

}  // namespace cent::app
