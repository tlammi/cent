#pragma once

#include <cent/dt/error.hpp>
#include <expected>

namespace cent {

struct value_t {};
constexpr value_t value{};
struct error_t {};
constexpr error_t error{};

template <class T>
class Result {
 public:
    constexpr Result() = default;

    constexpr Result(const T& other) : m_exp(other) {}
    constexpr Result(T&& other) : m_exp(std::move(other)) {}

    template <class... Ts>
    constexpr Result(value_t /*value*/, Ts&&... ts)
        : m_exp(std::in_place, std::forward<Ts>(ts)...) {}

    constexpr Result(Error e) : m_exp(std::unexpect, std::move(e)) {}
    constexpr Result(Errno e) : m_exp(std::unexpect, e) {}

    constexpr Result(const Result&) = default;
    constexpr Result& operator=(const Result&) = default;

    constexpr Result(Result&&) noexcept = default;
    constexpr Result& operator=(Result&&) noexcept = default;

    ~Result() = default;

    constexpr bool has_value() const noexcept { return m_exp.has_value(); }

    constexpr operator bool() const noexcept { return has_value(); }

    constexpr T* operator->() noexcept { return m_exp.operator->(); }
    constexpr const T* operator->() const noexcept {
        return m_exp.operator->();
    }

    constexpr T& operator*() noexcept { return m_exp.operator*(); }
    constexpr const T& operator*() const noexcept { return m_exp.operator*(); }
    constexpr const T& value() const { return m_exp.value(); }
    constexpr T& value() { return m_exp.value(); }
    constexpr const Error& error() const { return m_exp.error(); }

 private:
    std::expected<T, Error> m_exp{};
};
}  // namespace cent

