#pragma once

#include <cstdint>
#include <initializer_list>
#include <map>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

namespace cent::json {

using Null = std::nullptr_t;
using Bool = bool;
using Int = std::int64_t;
using Float = long double;
using Str = std::string;

class Arr;
class Obj;
using Value = std::variant<Null, Bool, Int, Float, Str, Arr, Obj>;
class Json;

class Arr {
 public:
    Arr();

    template <class... Ts>
    Arr(std::in_place_t, Ts&&... values)
        : m_values{Json(std::forward<Ts>(values))...} {}

    Arr(const Arr&) = default;
    Arr(Arr&&) = default;
    Arr& operator=(const Arr&) = default;
    Arr& operator=(Arr&&) = default;
    ~Arr() = default;

    auto begin() noexcept { return m_values.begin(); }
    auto end() noexcept { return m_values.end(); }
    auto begin() const noexcept { return m_values.begin(); }
    auto end() const noexcept { return m_values.end(); }
    auto cbegin() const noexcept { return m_values.cbegin(); }
    auto cend() const noexcept { return m_values.cend(); }

    bool empty() const noexcept { return m_values.empty(); }

    const Json& operator[](size_t index) const;
    Json& operator[](size_t index);

    const Json& front() const noexcept;
    const Json& back() const noexcept;

    size_t size() const noexcept;
    size_t length() const noexcept;

    void push_back(Json v);
    template <class T>
    void emplace_back(T&& v);

 private:
    std::vector<Json> m_values;
};

class Obj {
 public:
    Obj() = default;
    Obj(std::initializer_list<std::pair<std::string, Value>> values);

    Obj(const Obj&) = default;
    Obj(Obj&&) = default;
    Obj& operator=(const Obj&) = default;
    Obj& operator=(Obj&&) = default;
    ~Obj() = default;

    auto begin() noexcept { return m_values.begin(); }
    auto end() noexcept { return m_values.end(); }
    auto begin() const noexcept { return m_values.begin(); }
    auto end() const noexcept { return m_values.end(); }
    auto cbegin() const noexcept { return m_values.cbegin(); }
    auto cend() const noexcept { return m_values.cend(); }

    bool empty() const noexcept { return m_values.empty(); }
    size_t size() const noexcept { return m_values.size(); }
    size_t length() const noexcept { return m_values.size(); }

    Json& operator[](const std::string& key);
    const Json& operator[](const std::string& key) const;

 private:
    std::map<std::string, Json> m_values;
};

class Json {
 public:
    constexpr Json() noexcept = default;
    constexpr Json(Value v) noexcept : m_value{std::move(v)} {}
    constexpr Json(const Json&) = default;
    constexpr Json(Json&&) = default;
    constexpr Json& operator=(const Json&) = default;
    constexpr Json& operator=(Json&&) = default;
    constexpr ~Json() = default;

    constexpr size_t index() const noexcept { return m_value.index(); }

    constexpr bool is_null() const noexcept {
        return std::holds_alternative<Null>(m_value);
    }
    constexpr bool is_bool() const noexcept {
        return std::holds_alternative<Bool>(m_value);
    }
    constexpr bool is_int() const noexcept {
        return std::holds_alternative<Int>(m_value);
    }
    constexpr bool is_float() const noexcept {
        return std::holds_alternative<Float>(m_value);
    }
    constexpr bool is_str() const noexcept {
        return std::holds_alternative<Str>(m_value);
    }
    constexpr bool is_arr() const noexcept {
        return std::holds_alternative<Arr>(m_value);
    }
    constexpr bool is_obj() const noexcept {
        return std::holds_alternative<Obj>(m_value);
    }

    constexpr operator bool() const noexcept { return !is_null(); }

    constexpr Bool& as_bool() { return std::get<Bool>(m_value); }
    constexpr Bool as_bool() const { return std::get<Bool>(m_value); }
    constexpr Int& as_int() { return std::get<Int>(m_value); }
    constexpr Int as_int() const { return std::get<Int>(m_value); }
    constexpr Float& as_float() { return std::get<Float>(m_value); }
    constexpr Float as_float() const { return std::get<Float>(m_value); }
    constexpr Str& as_str() { return std::get<Str>(m_value); }
    constexpr const Str& as_str() const { return std::get<Str>(m_value); }
    constexpr Arr& as_arr() { return std::get<Arr>(m_value); }
    constexpr const Arr& as_arr() const { return std::get<Arr>(m_value); }
    constexpr Obj& as_obj() { return std::get<Obj>(m_value); }
    constexpr const Obj& as_obj() const { return std::get<Obj>(m_value); }

 private:
    Value m_value;
};

std::ostream& operator<<(std::ostream& s, const Json& j);

inline std::ostream& operator<<(std::ostream& s, const Arr& a) {
    s << "[";
    bool first = true;
    for (const auto& v : a) {
        if (!first) s << ", ";
        first = false;
        s << v;
    }
    s << "]";
    return s;
}

inline std::ostream& operator<<(std::ostream& s, const Obj& o) {
    s << "{";
    for (const auto& [k, v] : o) {
        if (k != o.begin()->first) s << ", ";
        s << R"(")" << k << R"(": )" << v;
    }
    s << "}";
    return s;
}

inline std::ostream& operator<<(std::ostream& s, const Json& j) {
    if (j.is_null())
        s << "null";
    else if (j.is_bool())
        s << (j.as_bool() ? "true" : "false");
    else if (j.is_int())
        s << j.as_int();
    else if (j.is_float())
        s << j.as_float();
    else if (j.is_str())
        s << j.as_str();
    else if (j.is_arr())
        s << j.as_arr();
    else if (j.is_obj())
        s << j.as_obj();
    return s;
}

Arr::Arr() : m_values() {}

const Json& Arr::operator[](size_t index) const { return m_values.at(index); }
Json& Arr::operator[](size_t index) { return m_values.at(index); }
const Json& Arr::front() const noexcept { return m_values.front(); }
const Json& Arr::back() const noexcept { return m_values.back(); }

size_t Arr::size() const noexcept { return m_values.size(); }
size_t Arr::length() const noexcept { return m_values.size(); }
void Arr::push_back(Json v) { m_values.push_back(std::move(v)); }

template <class T>
void Arr::emplace_back(T&& v) {
    m_values.emplace_back(std::forward<T>(v));
}

Obj::Obj(std::initializer_list<std::pair<std::string, Value>> values)
    : m_values(values.begin(), values.end()) {}

Json& Obj::operator[](const std::string& key) { return m_values[key]; }
const Json& Obj::operator[](const std::string& key) const {
    return m_values.at(key);
}

}  // namespace cent::json
