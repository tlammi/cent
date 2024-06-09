#pragma once

#include <span>
#include <string_view>
#include <utility>

namespace cent::cli {

enum class ArgType {
    End,
    Short,
    Long,
    Value,
};

struct Arg {
    ArgType type;
    std::string_view value;

    constexpr explicit operator bool() const { return type != ArgType::End; }
};

constexpr auto make_lexer(std::span<const char* const> spn) noexcept {
    return [spn, first = std::string_view()]() mutable -> Arg {
        using enum ArgType;
        if (!first.empty()) {
            if (first.front() == '=') {
                first.remove_prefix(1);
                return {Value, std::exchange(first, {})};
            }
            auto val = first.substr(0, 1);
            first.remove_prefix(1);
            return {Short, val};
        }
        if (spn.empty()) return {End, ""};
        first = spn.front();
        spn = spn.subspan(1);
        if (first.starts_with("--")) {
            first.remove_prefix(2);
            auto idx = first.find('=');
            if (idx == std::string_view::npos) {
                return {Long, std::exchange(first, {})};
            } else {
                auto val = first.substr(0, idx);
                first.remove_prefix(idx);
                return {Long, val};
            }
        } else if (first.starts_with('-')) {
            first.remove_prefix(1);
            auto val = first.substr(0, 1);
            first.remove_prefix(1);
            return {Short, val};
        } else {
            return {Value, std::exchange(first, {})};
        }
        return {};
    };
}

constexpr auto make_lexer(int argc, const char* const* argv) {
    return make_lexer(std::span<const char* const>(argv, argc));
}

}  // namespace cent::cli
