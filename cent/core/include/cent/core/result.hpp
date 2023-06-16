#pragma once

#include <cent/core/error.hpp>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <variant>

namespace cent {
namespace result_detail {
constexpr size_t value_idx = 0;
constexpr size_t error_idx = 1;
}  // namespace result_detail

using in_place_value_t = std::in_place_index_t<result_detail::value_idx>;
using in_place_error_t = std::in_place_index_t<result_detail::error_idx>;

constexpr in_place_value_t in_place_value{};
constexpr in_place_error_t in_place_error{};

template <class T>
class Result;

template <>
class Result<void> {
 public:
    constexpr bool valid() const noexcept { return false; }
    explicit constexpr operator bool() const noexcept { return false; }

    constexpr Error& error() noexcept { return m_e; }
    constexpr const Error& error() const noexcept { return m_e; }

 private:
    Error m_e{};
};

template <class T>
class Result {
    static constexpr auto value_idx = result_detail::value_idx;
    static constexpr auto error_idx = result_detail::error_idx;

 public:
    using value_type = T;
    using error_type = Error;
    Result() : Result(in_place_value) {}

    explicit Result(Error e) : Result(in_place_error, std::move(e)) {}
    explicit Result(Result<void> null) : Result(std::move(null.error())) {}

    template <class... Ts>
    explicit Result(in_place_value_t /*unused*/, Ts&&... ts)
        : m_v{in_place_value, std::forward<Ts>(ts)...} {}

    template <class... Ts>
    explicit Result(in_place_error_t /*unused*/, Ts&&... ts)
        : m_v{in_place_error, std::forward<Ts>(ts)...} {}

    constexpr bool valid() const noexcept { return m_v.index() == value_idx; }
    explicit constexpr operator bool() const noexcept { return valid(); }

    constexpr T& operator*() { return std::get<value_idx>(m_v); }
    constexpr const T& operator*() const { return std::get<value_idx>(m_v); }

    constexpr T* operator->() { return &std::get<value_idx>(m_v); }
    constexpr const T* operator->() const { return &std::get<value_idx>(m_v); }

    constexpr Error& error() { return std::get<error_idx>(m_v); }
    constexpr const Error& error() const { return std::get<error_idx>(m_v); }

    template <class F, class Res = std::invoke_result_t<F, value_type&&>>
    auto translate(F&& f) && {
        return Result<Res>{in_place_value, f(std::move(**this))};
    }

    template <class F, class Res = std::invoke_result_t<F, const value_type&>>
    auto translate(F&& f) const& {
        return Result<Res>{in_place_value, f(**this)};
    }

    template <class F, class Res = std::invoke_result_t<F, value_type&>>
    auto translate(F&& f) & {
        return Result<Res>{in_place_value, f(**this)};
    }

 private:
    std::variant<T, Error> m_v{};
};

using UnitResult = Result<std::monostate>;
using NullResult = Result<void>;

template <class T, class... Ts>
Result<T> make_value(Ts&&... ts) {
    return Result<T>{in_place_value, std::forward<Ts>(ts)...};
}

template <class... Ts>
NullResult make_error(Ts&&... ts) {
    return NullResult{std::forward<Ts>(ts)...};
}

}  // namespace cent
