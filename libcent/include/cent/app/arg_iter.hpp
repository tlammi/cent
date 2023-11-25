#pragma once

#include <cent/util/cast.hpp>
#include <iostream>
#include <string_view>

namespace cent::app {

enum class OptType {
    Short,
    Long,
    Val,
};

struct Opt {
    OptType type;
    std::string_view val;

    constexpr bool operator==(const Opt& other) const noexcept {
        return type == other.type && val == other.val;
    }
};

inline std::ostream& operator<<(std::ostream& s, const Opt& o) {
    s << "(" << underlying_cast(o.type) << ", " << o.val << ")";
    return s;
}

template <class CharT>
class ArgIter {
 public:
    constexpr ArgIter() = default;
    constexpr ArgIter(size_t argc, CharT** argv) : m_argc{argc}, m_argv{argv} {
        if (!m_argc) return;
        m_buf = *m_argv;
        load_next();
    }

    constexpr ArgIter(int argc, CharT** argv)
        : ArgIter(static_cast<size_t>(argc), argv) {}

    constexpr bool operator==(const ArgIter& other) const {
        return (m_argc == 0 && other.m_argc == 0) || m_argv == other.m_argv;
    }

    constexpr bool operator!=(const ArgIter& other) const {
        return !(*this == other);
    }

    constexpr ArgIter& operator++() {
        load_next();
        return *this;
    }

    constexpr Opt operator*() const noexcept { return m_curr; }

    constexpr const Opt* operator->() const noexcept { return &m_curr; }

 private:
    void load_next() {
        std::cerr << "loading\n";
        if (m_end_reached) {
            std::cerr << "end reached\n";
            ++m_argv;
            --m_argc;
            if (!m_argc) return;
            m_buf = *m_argv;
            m_end_reached = false;
            m_short_flag = false;
        }
        if (m_buf.starts_with("--")) {
            m_buf.remove_prefix(2);
            m_short_flag = false;
            auto i = m_buf.find('=');
            if (i == std::string_view::npos) {
                m_end_reached = true;
                m_curr = {OptType::Long, m_buf};
                m_buf = "";
            } else {
                m_curr = {OptType::Long, m_buf.substr(0, i)};
                m_buf = m_buf.substr(i + 1);
            }
        } else if (m_buf.starts_with('-')) {
            m_buf.remove_prefix(1);
            m_short_flag = true;
            m_curr = {OptType::Short, m_buf.substr(0, 1)};
            m_buf.remove_prefix(1);
            m_end_reached = m_buf.empty();
        } else if (m_short_flag) {
            std::cerr << "was short flag\n";
            if (m_buf.front() == '=') {
                m_buf.remove_prefix(1);
                m_short_flag = false;
                m_curr = {OptType::Val, m_buf};
                m_buf = "";
                m_end_reached = true;
                return;
            }
            m_curr = {OptType::Short, m_buf.substr(0, 1)};
            m_buf.remove_prefix(1);
            m_end_reached = m_buf.empty();
        } else {
            std::cerr << "val: " << m_buf << '\n';
            m_curr = {OptType::Val, m_buf};
            m_buf = "";
            m_end_reached = true;
        }
    }
    size_t m_argc{};
    CharT** m_argv{};
    std::string_view m_buf{};
    bool m_short_flag{false};
    bool m_end_reached{false};
    Opt m_curr{};
};
}  // namespace cent::app
