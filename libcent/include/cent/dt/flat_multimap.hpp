#pragma once

#include <algorithm>
#include <span>
#include <utility>
#include <vector>

namespace cent {

struct FlatMultimapDefaultCompare {
    template <class K, class L>
    constexpr bool operator()(const K& key, const L& lookup) const noexcept {
        return key == lookup;
    }
};

template <class K, class V, class Compare = FlatMultimapDefaultCompare>
class FlatMultimap {
    using Pair = std::pair<K, V>;
    using Vec = std::vector<Pair>;

 public:
    using value_type = Pair;
    using iterator = Vec::iterator;
    using const_iterator = Vec::const_iterator;

    constexpr FlatMultimap() noexcept = default;

    FlatMultimap(std::initializer_list<value_type> il) : m_vec{il} {}

    iterator begin() noexcept { return m_vec.begin(); }
    const_iterator begin() const noexcept { return m_vec.begin(); }
    const_iterator cbegin() const noexcept { return m_vec.begin(); }

    iterator end() noexcept { return m_vec.end(); }
    const_iterator end() const noexcept { return m_vec.end(); }
    const_iterator cend() const noexcept { return m_vec.end(); }

    template <class... Ts>
    void emplace(Ts&&... ts) {
        m_vec.emplac_back(std::forward<Ts>(ts)...);
    }

    template <class L>
    iterator find(const L& lookup) noexcept {
        return std::find_if(m_vec.begin(), m_vec.end(), [&](const auto& p) {
            return Compare()(p.first, lookup);
        });
    }

    template <class L>
    const_iterator find(const L& lookup) const noexcept {
        return std::find_if(m_vec.begin(), m_vec.end(), [&](const auto& p) {
            return Compare()(p.first, lookup);
        });
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return m_vec.empty();
    }

    template <class L>
    [[nodiscard]] bool contains(const L& lookup) const noexcept {
        return find(lookup) != end();
    }

    template <class L>
    [[nodiscard]] size_t count(const L& lookup) const noexcept {
        return std::count_if(begin(), end(), [&](const auto& p) {
            return Compare()(p.first, lookup);
        });
    }

 private:
    Vec m_vec{};
};
}  // namespace cent
