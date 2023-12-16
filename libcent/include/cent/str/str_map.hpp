#pragma once

#include <iostream>
#include <string_view>

namespace cent {

template <class Str>
class BasicStrMap {
 public:
    using string_type = Str;
    using value_type = std::pair<std::string_view, std::string_view>;

    class Iterator {
     public:
        constexpr Iterator(std::string_view str) : m_str(str) {}

        constexpr Iterator() = default;

        constexpr bool operator==(const Iterator& other) const noexcept {
            return m_str.size() == other.m_str.size();
        }

        constexpr value_type operator*() const noexcept {
            if (m_key_idx < m_str.size())
                return {m_str.substr(0, m_key_idx),
                        m_str.substr(m_key_idx + 1, m_val_idx - m_key_idx - 1)};
            return {m_str, ""};
        }

        constexpr Iterator& operator++() noexcept {
            if (m_val_idx < m_str.size()) {
                m_str = m_str.substr(m_val_idx + 1);
                m_key_idx = key_idx();
                m_val_idx = val_idx(m_key_idx);
            } else {
                m_str = "";
                m_key_idx = 0;
                m_val_idx = 0;
            }
            return *this;
        }

     private:
        constexpr size_t key_idx() const noexcept {
            auto idx = m_str.find('=');
            if (idx == std::string_view::npos) return m_str.size();
            return idx;
        }

        constexpr size_t val_idx(size_t start) {
            auto idx = m_str.substr(start).find(',');
            if (idx == std::string_view::npos) return m_str.size();
            return idx + start;
        }

        std::string_view m_str{};
        size_t m_key_idx{key_idx()};
        size_t m_val_idx{val_idx(m_key_idx)};
    };

    constexpr BasicStrMap() = default;

    constexpr BasicStrMap(string_type s) : m_str(std::move(s)) {}

    constexpr Iterator begin() const noexcept { return {m_str}; }
    constexpr Iterator cbegin() const noexcept { return {m_str}; }
    constexpr Iterator end() const noexcept { return {}; }
    constexpr Iterator cend() const noexcept { return {}; }

 private:
    string_type m_str{};
};

using StrMap = BasicStrMap<std::string_view>;

}  // namespace cent
