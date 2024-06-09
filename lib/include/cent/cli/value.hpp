#pragma once

#include <algorithm>
#include <cent/types.hpp>
#include <ranges>
#include <string_view>
#include <vector>

namespace cent::cli {
namespace detail {

namespace rv = std::ranges::views;

template <class T>
constexpr Result<T> parse(std::string_view s) {
    using namespace std::string_view_literals;
    if constexpr (std::is_same_v<T, bool>) {
        static constexpr auto to_lower = [](char c) noexcept -> char {
            if (c >= 'A' && c <= 'Z') return static_cast<char>(c - 'A' + 'a');
            return c;
        };
        auto lower = rv::transform(s, to_lower);
        if (std::ranges::equal(lower, "true"sv)) { return {true}; }
        if (std::ranges::equal(lower, "false"sv)) { return {false}; }
        if (std::ranges::equal(lower, "1"sv)) { return {true}; }
        if (std::ranges::equal(lower, "0"sv)) { return {false}; }
        return error(ErrorCode::Inval);
    } else if constexpr (std::integral<T>) {
        T value{};
        for (auto c : s) {
            if (c < '0' || c > '9') return error(ErrorCode::Inval);
            value *= 10;  // NOLINT
            value += c - '0';
        }
        return value;
    } else if (std::same_as<T, std::string_view>) {
        return {s};
    }
}

}  // namespace detail

enum class Nargs {
    None,  // no arguments (switches)
    One,   // a single argument
    Some,  // 0 -> inf
    More,  // 1 -> inf
};

class IValue {
 public:
    virtual constexpr ~IValue() = default;
    virtual Result<void> parse(std::string_view s) noexcept = 0;

    virtual Nargs nargs() const noexcept = 0;

 private:
};

template <class T>
struct Value final : public IValue {
    T* value;

    explicit constexpr Value(T* store) noexcept : value{store} {}

    Result<void> parse(std::string_view s) noexcept override {
        return detail::parse<T>(s).and_then([&](auto&& val) {
            *value = val;
            return Result<void>();
        });
    }

    Nargs nargs() const noexcept override { return Nargs::One; }
};

template <class T>
struct is_vector : std::false_type {};

template <class T>
struct is_vector<std::vector<T>> : std::true_type{};

template <concepts::vector T>
struct Value<T> final : public IValue {
    T* value;
    explicit constexpr Value(T* value) noexcept : value(value) {}

    Result<void> parse(std::string_view s) noexcept override {
        return detail::parse<typename std::remove_cvref_t<T>::value_type>(s)
            .and_then([&](auto&& v) {
                value->push_back(std::forward<decltype(v)>(v));
                return Result<void>();
            });
    }

    Nargs nargs() const noexcept override { return Nargs::Some; }
};

}  // namespace cent::cli
