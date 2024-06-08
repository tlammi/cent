#pragma once

#include <cent/types.hpp>
#include <string_view>
#include <vector>

namespace cent::cli {

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

template <std::integral T>
struct Value final : public IValue {
    T* value;

    explicit constexpr Value(T* store) noexcept : value{store} {}

    Result<void> parse(std::string_view s) noexcept override {
        for (auto c : s) {
            if (c < '0' || c > '9') return error(ErrorCode::Inval);
            *value *= 10;  // NOLINT
            *value += c - '0';
        }
        return {};
    }

    Nargs nargs() const noexcept override { return Nargs::One; }
};

template <class T>
struct is_vector : std::false_type {};

template <class T>
struct is_vector<std::vector<T>> : std::true_type{};

}  // namespace cent::cli
