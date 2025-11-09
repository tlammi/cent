#pragma once

#include <cent/static_str.hpp>
#include <cstring>
#include <string>
#include <string_view>
#include <utility>

namespace cent {
namespace const_str_detail {

inline std::string_view dupstr(std::string_view str) {
    return {::strdup(str.data()), str.size()};
}
}  // namespace const_str_detail

class ConstStr {
 public:
    constexpr ConstStr() noexcept = default;

    explicit constexpr ConstStr(StaticStr str)
        : m_str(str.string_view()), m_alloc(false) {}

    explicit ConstStr(const std::string& str)
        : m_str{const_str_detail::dupstr(str)}, m_alloc(true) {}

    constexpr ConstStr(const ConstStr& other)
        : m_str(other.m_alloc ? const_str_detail::dupstr(other.m_str)
                              : other.m_str),
          m_alloc(other.m_alloc) {}

    constexpr ConstStr& operator=(const ConstStr& other) {
        std::destroy_at(this);
        std::construct_at(this, other);
        return *this;
    }

    constexpr ConstStr(ConstStr&& other) noexcept
        : m_str(other.m_str), m_alloc(std::exchange(other.m_alloc, false)) {}

    constexpr ConstStr& operator=(ConstStr&& other) noexcept {
        std::destroy_at(this);
        std::construct_at(this, std::move(other));
        return *this;
    }

    constexpr ~ConstStr() {
        if (m_alloc) delete m_str.data();
    }

    constexpr operator std::string_view() const noexcept { return m_str; }

    constexpr const char* c_str() const noexcept { return m_str.data(); }

 private:
    std::string_view m_str{};
    bool m_alloc{};
};
}  // namespace cent
