#pragma once

#include <concepts>
#include <exception>

namespace cent {

template <class T>
concept exception_member = requires(const T t) {
    { t.what() } -> std::same_as<const char*>;
};

template <exception_member T, class Category = void>
class Exception final : public Exception<T, void> {
 public:
    using Exception<T, void>::Exception;
};

template <exception_member T>
class Exception<T, void> : public std::exception {
 public:
    constexpr explicit Exception(T t) noexcept : m_t(std::move(t)) {}

    constexpr const char* what() const noexcept override { return m_t.what(); }

    constexpr const T& value() const noexcept { return m_t; }

 private:
    T m_t;
};
}  // namespace cent
