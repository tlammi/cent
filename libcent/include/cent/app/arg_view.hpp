#pragma once

#include <cstddef>
#include <limits>

namespace cent::app {

template <class CharT>
class ArgView {
 public:
    static constexpr size_t npos = std::numeric_limits<size_t>::max();

    constexpr ArgView() = default;
    constexpr ArgView(size_t argc, CharT** argv) : m_ac{argc}, m_av{argv} {}

    constexpr operator bool() const noexcept { return !empty(); }
    [[nodiscard]] constexpr bool empty() const noexcept { return m_ac == 0; }

    [[nodiscard]] constexpr ArgView subview(
        size_t offset, size_t count = npos) const noexcept {
        if (count == npos) return {m_ac - offset, m_av + offset};  // NOLINT
        return {count, m_av + offset};                             // NOLINT
    }

    constexpr void remove_prefix(size_t count = 1) noexcept {
        m_ac -= count;
        m_av += count;  // NOLINT
    }

    constexpr void remove_suffix(size_t count = 1) noexcept { m_ac -= count; }

    [[nodiscard]] constexpr size_t size() const noexcept { return m_ac; }
    [[nodiscard]] constexpr size_t length() const noexcept { return size(); }

    [[nodiscard]] CharT* front() const noexcept { return m_av[0]; }  // NOLINT

 private:
    size_t m_ac{};
    CharT** m_av{};
};
}  // namespace cent::app
