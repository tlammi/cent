#pragma once

#include <stdexcept>
#include <string>

namespace cent::http {

template <class StringT>
class BasicHeader {
 public:
    constexpr BasicHeader() : BasicHeader("") {}
    constexpr BasicHeader(StringT value) : m_value{std::move(value)} {
        if (m_value.empty()) m_value = ":";
        const auto idx = m_value.find(':');
        if (idx == StringT::npos) throw std::runtime_error("Invalid header");
    }

    constexpr BasicHeader& operator=(StringT value) {
        if (value.empty()) m_value = ":";
        const auto idx = value.find(':');
        if (idx == StringT::npos) throw std::runtime_error("Invalid header");
        m_value = std::move(value);
        return *this;
    }

    constexpr std::string_view sub_value(std::string_view field) {
        auto idx = m_value.find(field);
        if (idx == StringT::npos) return "";
        idx += field.size();
        if (idx >= m_value.size()) return "";
        if (m_value[idx] != '=') return "";
        ++idx;
        if (idx >= m_value.size()) return "";
        std::string_view value = std::string_view(m_value).substr(idx);
        auto end = value.find(',', 1);
        value = value.substr(0, end);
        while (!value.empty() && std::isspace(value.back()))
            value.remove_suffix(1);
        return value;
    }

    constexpr bool header_equals(std::string_view hdr) const noexcept {
        auto myhdr = field_name();
        if (myhdr.size() != hdr.size()) return false;
        for (size_t i = 0; i < myhdr.size(); ++i) {
            if (std::tolower(myhdr[i]) != std::tolower(hdr[i])) return false;
        }
        return true;
    }

    constexpr std::string_view field_name() const noexcept {
        const auto idx = m_value.find(':');
        return std::string_view(m_value).substr(0, idx);
    }

    constexpr std::string_view value() const noexcept {
        auto idx = m_value.find(':');
        std::string_view res = std::string_view(m_value).substr(idx + 1);
        while (!res.empty() && std::isspace(res.front())) res.remove_prefix(1);
        while (!res.empty() && std::isspace(res.back())) res.remove_suffix(1);
        return res;
    }

    constexpr const StringT& str() const noexcept { return m_value; }

 private:
    StringT m_value;
};

template <class T>
std::ostream& operator<<(std::ostream& os, const BasicHeader<T>& bh) {
    os << bh.str();
    return os;
}

using Header = BasicHeader<std::string>;
using HeaderView = BasicHeader<std::string_view>;

}  // namespace cent