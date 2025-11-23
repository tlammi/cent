#pragma once

#include <cent/error.hpp>
#include <string>
#include <string_view>

namespace cent {

/**
 * \brief Holds a container image name
 *
 * Name is something like docker.io/library/alpine:latest. This object does not
 * do any automatic deduction like "docker.io/alpine" ->
 * "docker.io/alpine:latest". That has to be done beforehand.
 *
 * The default reference is "latest" if none are specified.
 * */
template <class T>
class BasicName {
    template <class U>
    friend class BasicName;

 public:
    static constexpr bool is_view = std::same_as<T, std::string_view>;

    constexpr BasicName() noexcept = default;
    constexpr explicit(!is_view) BasicName(T v) : m_v(std::move(v)) {
        m_reg_end = m_v.find('/');
        if (m_reg_end == T::npos)
            raise(ErrorCode::FormatError, "Invalid OCI reference '{}'", m_v);
        auto it = std::ranges::find_if(
            m_v, [](char c) { return c == '@' || c == ':'; });
        if (it == m_v.end())
            m_repo_end = std::string_view::npos;
        else
            m_repo_end = it - m_v.begin();
    }

    constexpr BasicName(T v, size_t reg_end, size_t repo_end) noexcept
        : m_v(std::move(v)), m_reg_end(reg_end), m_repo_end(repo_end) {}

    template <class U>
        requires(!std::same_as<U, T>)
    constexpr explicit BasicName(BasicName<U> view)
        requires(!is_view)
        : BasicName(T{view.m_v}, view.m_reg_end, view.m_repo_end) {}

    constexpr BasicName(const BasicName&) = default;
    constexpr BasicName& operator=(const BasicName&) = default;

    constexpr BasicName(BasicName&&) noexcept = default;
    constexpr BasicName& operator=(BasicName&&) noexcept = default;

    constexpr ~BasicName() = default;

    constexpr std::string_view string_view() const noexcept { return m_v; }

    constexpr std::string_view registry() const noexcept {
        return string_view().substr(0, m_reg_end);
    }

    constexpr operator BasicName<std::string_view>() const noexcept
        requires(!is_view)
    {
        return {m_v, m_reg_end, m_repo_end};
    }

    constexpr std::string_view repository() const noexcept {
        if (m_v.empty()) return {};
        auto start = m_reg_end + 1;
        auto len = m_repo_end - start;
        return string_view().substr(start, len);
    }

    constexpr std::string_view tag() const noexcept {
        if (m_repo_end == std::string_view::npos)
            return std::string_view("latest");
        if (m_v[m_repo_end] == ':') return string_view().substr(m_repo_end + 1);
        return {};
    }

    constexpr std::string_view digest() const noexcept {
        if (m_repo_end == std::string_view::npos) return {};
        if (m_v[m_repo_end] == '@') return string_view().substr(m_repo_end + 1);
        return {};
    }

    constexpr std::string_view suffix() const noexcept {
        if (m_repo_end == std::string_view::npos) return "latest";
        return string_view().substr(m_repo_end + 1);
    }

    constexpr bool has_tag() const noexcept {
        if (m_repo_end == std::string_view::npos) return true;
        return m_v[m_repo_end] == ':';
    }

    constexpr void set_tag(std::string_view new_tag)
        requires(!is_view)
    {
        // TODO: Exceptions might corrupt
        if (m_repo_end != std::string_view::npos) m_v.resize(m_repo_end);
        m_repo_end = std::string_view::npos;
        if (!new_tag.empty()) {
            m_v.push_back(':');
            m_repo_end = m_v.size() - 1;
            m_v.append(new_tag);
        }
    }
    constexpr void set_digest(std::string_view new_digest)
        requires(!is_view)
    {
        // TODO: Exceptions might corrupt
        if (m_repo_end != std::string_view::npos) m_v.resize(m_repo_end);
        m_repo_end = std::string_view::npos;
        if (!new_digest.empty()) {
            m_v.push_back('@');
            m_repo_end = m_v.size() - 1;
            m_v.append(new_digest);
        }
    }

    template <class U>
    constexpr auto operator==(const BasicName<U>& other) const noexcept {
        return registry() == other.registry() &&
               repository() == other.repository() && suffix() == other.suffix();
    }

 private:
    T m_v{};
    size_t m_reg_end{std::string_view::npos};
    size_t m_repo_end{std::string_view::npos};
};

using Name = BasicName<std::string>;
using NameView = BasicName<std::string_view>;

namespace literals {
constexpr auto operator""_nm(const char* ptr, std::size_t len) {
    return NameView(std::string_view(ptr, len));
}
}  // namespace literals

}  // namespace cent
