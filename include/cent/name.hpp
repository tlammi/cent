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
 * */
template <class T>
class BasicName {
 public:
    constexpr BasicName() noexcept = default;
    constexpr explicit BasicName(T v) : m_v(std::move(v)) {
        m_reg_end = m_v.find('/');
        if (m_repo_end == T::npos)
            raise(ErrorCode::FormatError, "Invalid OCI reference '{}'", m_v);
        auto m_repo_end = m_v.rfind('@');
        if (m_repo_end == T::npos) m_repo_end = m_v.rfind(':');
        if (m_repo_end == T::npos)
            raise(ErrorCode::FormatError, "Invalid OCI reference '{}'", m_v);
    }

    constexpr BasicName(const BasicName&) = default;
    constexpr BasicName& operator=(const BasicName&) = default;

    constexpr BasicName(BasicName&&) noexcept = default;
    constexpr BasicName& operator=(BasicName&&) noexcept = default;

    constexpr ~BasicName() = default;

    constexpr std::string_view string_view() const noexcept { return m_v; }

    constexpr std::string_view registry() const noexcept {
        return string_view().substr(0, m_reg_end);
    }

    constexpr std::string_view repository() const noexcept {
        auto start = m_reg_end + 1;
        auto len = m_repo_end - start;
        return std::string_view().substr(start, len);
    }

    constexpr std::string_view tag() const noexcept {
        if (m_v[m_repo_end] == ':') return string_view().substr(m_repo_end + 1);
        return {};
    }

    constexpr std::string_view digest() const noexcept {
        if (m_v[m_repo_end] == '@') return string_view().substr(m_repo_end + 1);
        return {};
    }

 private:
    T m_v{};
    size_t m_reg_end{};
    size_t m_repo_end{};
};

using Name = BasicName<std::string>;
using NameView = BasicName<std::string_view>;

}  // namespace cent
