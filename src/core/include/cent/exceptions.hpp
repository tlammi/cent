#pragma once

#include <cent/static_str.hpp>
#include <exception>
#include <format>
#include <string>

namespace cent {

class Exception : public std::exception {};

class MessagedException : public Exception {
 public:
    explicit MessagedException(std::string s) : m_what(std::move(s)) {}

    constexpr const char* what() const noexcept override {
        return m_what.c_str();
    }

 private:
    std::string m_what;
};

#define EX(type, msg)                                                      \
    class type : public MessagedException {                                \
     public:                                                               \
        constexpr type() : MessagedException(msg) {}                       \
        constexpr type(std::string s) : MessagedException(std::move(s)) {} \
    }

EX(PermissionDenied, "Permission denied");

template <class Ex, class... Ts>
    requires(sizeof...(Ts) > 0)
[[noreturn]] constexpr void raise(std::format_string<Ts...> fmt, Ts&&... ts) {
    throw Ex(std::format(fmt, std::forward<Ts>(ts)...));
}

}  // namespace cent
