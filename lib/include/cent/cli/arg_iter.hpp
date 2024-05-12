#pragma once
#include <iterator>
#include <span>

namespace cent::cli {

class ArgIter {
 public:
    using difference_type = std::ptrdiff_t;
    using value_type = std::string_view;
    using reference = std::string_view;

    constexpr ArgIter() noexcept = default;
    constexpr explicit ArgIter(std::span<const char*> args) noexcept
        : m_args(args) {
        load_next();
    }

    constexpr bool operator==(const ArgIter& other) const noexcept {
        return m_args.size() == other.m_args.size();
    }

    constexpr ArgIter& operator++() noexcept {
        load_next();
        return *this;
    }

    constexpr ArgIter operator++(int) noexcept {
        ArgIter tmp = *this;
        ++(*this);
        return tmp;
    }

    constexpr std::string_view operator*() const noexcept { return m_val; }

 private:
    constexpr void load_next() noexcept {
        if (m_args.empty()) return;
        std::string_view val = m_args.front();
        if (val.starts_with("--")) {
            auto idx = val.find('=');
            if (idx == std::string_view::npos) {
                m_val = val;
                m_args = m_args.subspan(1);
                return;
            }
            m_val = val.substr(0, idx);
            m_args.front() = &m_args.front()[idx + 1];
            return;
        }
        m_val = val;
        m_args = m_args.subspan(1);
    }
    std::span<const char*> m_args{};
    std::string_view m_val{};
};

constexpr auto arg_range(std::span<const char*> args) {
    struct Range {
        std::span<const char*> args;
        ArgIter begin() const noexcept { return ArgIter{args}; }
        ArgIter end() const noexcept { return ArgIter{}; }
    };
    return Range{args};
}

}  // namespace cent::cli
