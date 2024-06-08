#pragma once

#include <cent/str/c_str.hpp>
#include <cent/util.hpp>
#include <cstddef>
#include <string_view>

namespace cent::str {
template <size_t S>
using StrLit = const char[S];
namespace detail {

template <const std::string_view&... Strs>
struct toltal_len {
    static constexpr size_t value = (0 + ... + Strs.size());
};

template <const std::string_view&... Strs>
constexpr size_t total_len_v = toltal_len<Strs...>::value;

template <size_t... Ss>
struct total_strlit_len {
    static constexpr size_t value = (0 + ... + Ss) - sizeof...(Ss) + 1;
};

template <size_t... Ss>
constexpr size_t total_strlit_len_v = total_strlit_len<Ss...>::value;

template <class F, class... Ts>
constexpr void str_cpy(char* dst, F&& f, Ts&&... ts) {
    auto s = std::string_view(std::forward<F>(f));
    for (size_t i = 0; i < s.size(); ++i) { dst[i] = s[i]; }
    if constexpr (sizeof...(Ts)) {
        str_cpy(dst + s.size(), std::forward<Ts>(ts)...);
    }
}

}  // namespace detail

template <size_t S>
struct size_type_t {};

template <size_t S>
constexpr size_type_t<S> size_type{};

template <size_t S>
class StaticStrArr {
 public:
    constexpr StaticStrArr() noexcept = default;

    template <size_t... Ss>
    consteval StaticStrArr(StrLit<Ss>&... strs) noexcept {
        detail::str_cpy(m_s, strs...);
        m_s[S - 1] = '\0';
    }

    template <class... Strs>
    consteval StaticStrArr(size_type_t<S>, Strs... strs) {
        if (S <= (0 + ... + strs.size())) { panic(); }
        detail::str_cpy(m_s, strs...);
        m_s[S - 1] = '\0';
    }

    constexpr const char* c_str() const noexcept { return m_s; }
    constexpr std::string_view view() const noexcept { return {m_s, S - 1}; }

    constexpr size_t size() const noexcept { return S; }

    constexpr bool operator==(const StaticStrArr& other) const noexcept {
        return view() == other.view();
    }

    template <size_t S2>
    constexpr bool operator==(StrLit<S2>& other) const noexcept {
        if constexpr (S != S2)
            return false;
        else
            return view() == other;
    }

 private:
    char m_s[S]{};
};

template <size_t... Ss>
StaticStrArr(StrLit<Ss>&...) -> StaticStrArr<detail::total_strlit_len_v<Ss...>>;

class StaticStr {
 public:
    constexpr StaticStr() noexcept = default;

    consteval StaticStr(CStr s) : m_s(s.c_str()) {}

    template <size_t S>
    consteval StaticStr(const StrLit<S>& s) noexcept : m_s(s) {}

    template <size_t S>
    consteval StaticStr(const StaticStrArr<S>& s) noexcept
        : StaticStr(s.view()) {}

    constexpr const char* c_str() const noexcept { return m_s.c_str(); }
    constexpr std::string_view view() const noexcept { return m_s.view(); }

    [[nodiscard]] constexpr bool empty() const noexcept { return m_s.empty(); }

 private:
    CStr m_s{};
};

namespace literals {
consteval StaticStr operator""_static(const char* s, size_t l) noexcept {
    (void)l;
    return StaticStr{s};
}
}  // namespace literals

}  // namespace cent::str
