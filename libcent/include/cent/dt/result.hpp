#pragma once

#include <cent/dt/error.hpp>
#include <cent/util/raise.hpp>
#include <system_error>
#include <utility>

// NOLINTBEGIN(cppcoreguidelines-pro-type-union-access)

namespace cent {

struct value_t {};
constexpr value_t value{};
struct error_t {};
constexpr error_t error{};

template <class T>
class Result {
 public:
    constexpr Result() : m_value{}, m_has_value(true) {}

    constexpr Result(const T& value) : m_value{value}, m_has_value(true) {}
    constexpr Result(T&& value)
        : m_value{std::move(value)}, m_has_value(true) {}

    template <class... Ts>
    constexpr Result(value_t /*unused*/, Ts&&... values)
        : m_value{std::forward<Ts>(values)...}, m_has_value(true) {}

    constexpr Result(Errno e) : m_err{e}, m_has_value(false) {}

    constexpr Result(Error e) : m_err(e), m_has_value(false) {}

    constexpr Result(Result&& other) noexcept : m_has_value(other.m_has_value) {
        if (other.m_has_value) new (&m_value) T(std::move(other.m_value));
    }

    constexpr Result& operator=(Result&& other) noexcept {
        if (m_has_value) m_value.~T();
        m_has_value = other.m_has_value;
        if (other.m_has_value) new (&m_value) T(std::move(other.m_value));
        return *this;
    }

    constexpr ~Result() {
        if (m_has_value)
            m_value.~T();
        else
            m_err.~Error();
    }

    constexpr bool has_value() const noexcept { return m_has_value; }
    constexpr operator bool() const noexcept { return m_has_value; }

    constexpr T& operator*() noexcept { return m_value; }
    constexpr const T& operator*() const noexcept { return m_value; }
    constexpr T* operator->() noexcept { return &m_value; }
    constexpr const T* operator->() const noexcept { return &m_value; }

    const Error& error() const noexcept {
        if (m_has_value) raise("Has value");
        return m_err;
    }

    T& unwrap() {
        if (!m_has_value) raise("Has error: ", m_err.message());
        return m_value;
    }

 private:
    union {
        T m_value;
        Error m_err;
    };
    bool m_has_value{false};
};

// NOLINTEND(cppcoreguidelines-pro-type-union-access)

}  // namespace cent
