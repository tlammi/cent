#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace cent::sd {
using Null = std::nullptr_t;
using Bool = bool;
using Int = int64_t;
using Float = long double;
using Str = std::string;
class Json;
using Arr = std::vector<Json>;
using Dict = std::map<std::string, Json>;

using Value = std::variant<Null, Bool, Int, Float, Str, Arr, Dict>;

class Json {
 public:
    constexpr Json() = default;

    constexpr bool is_null() const noexcept {
        return std::holds_alternative<Null>(m_val);
    }

    constexpr bool is_bool() const noexcept {
        return std::holds_alternative<Bool>(m_val);
    }

    constexpr bool is_int() const noexcept {
        return std::holds_alternative<Int>(m_val);
    }

    constexpr bool is_float() const noexcept {
        return std::holds_alternative<Float>(m_val);
    }

 private:
    Value m_val{};
};

}  // namespace cent::sd
