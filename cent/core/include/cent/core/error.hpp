#pragma once

#include <cent/core/logs.hpp>
#include <stdexcept>
#include <string>
namespace cent {
class Exception final : public std::runtime_error {
 public:
    Exception(const char* what) : std::runtime_error(what) {}
    Exception(const std::string& what) : std::runtime_error(what) {}
};

class Error {
 public:
    Error();
    explicit Error(std::string_view s);
    explicit Error(const char* s);
    explicit Error(std::string s);
    [[noreturn]] void raise() const;

 private:
    std::string m_what;
};

// NOLINTNEXTLINE
#define CENT_FATAL(...)        \
    [&] {                      \
        CENT_ERR(__VA_ARGS__); \
        ::cent::logs_flush();  \
        ::std::terminate();    \
    }()

}  // namespace cent
