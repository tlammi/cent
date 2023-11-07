
#include <cent/sd/json.hpp>
#include <cent/str.hpp>
#include <iostream>

#include "json_lex.hpp"

namespace cent::sd::json {
using namespace cent::str::literals;
namespace {
using Iter = std::string_view::const_iterator;

template <class... Ts>
constexpr auto mk_err(const auto& lexer, str::StaticStr msg) {
    auto [line, col] = lexer.pos();
    return std::expected<Json, ParseError>{std::unexpect, msg.view(), line,
                                           col};
}

constexpr Int to_int(std::string_view s) {
    Int out{};
    bool neg = false;
    if (s.front() == '-') {
        s = s.substr(1);
        neg = true;
    }
    for (const auto c : s) {
        out = out * 10 + (c - '0');  // NOLINT
    }
    if (neg) return -out;
    return out;
}

constexpr Float to_float(std::string_view s) {
    Float out{};
    size_t i = 0;
    for (const auto c : s) {
        if (c == '.' || c == 'e' || c == 'E') break;
        ++i;
        out = out * 10 + (c - '0');  // NOLINT
    }
    if (i == s.size()) return out;
    if (s.at(i) == '.') {
        Float div = 0.1;  // NOLINT
        ++i;
        for (const auto c : s.substr(i)) {
            if (c == 'e' || c == 'E') break;
            ++i;
            out += div * (c - '0');  // NOLINT
            div *= 0.1;              // NOLINT
        }
    }
    if (i == s.size()) return out;
    if (s.at(i) == 'e' || s.at(i) == 'E') {
        ++i;
        if (s.at(i) == '-') {
        } else {
            if (s.at(i) == '+') ++i;
            auto exp = to_int(s.substr(i));
            if (exp == 0) return 1.0;
            Float mul = 1.0;
            while (exp--) mul *= 10.0;  // NOLINT
            return out * mul;
        }
    }
    return out;
}
std::expected<Json, ParseError> parse_recurse(Lexer<Iter>& lexer);
std::expected<Json, ParseError> parse_recurse_obj(Lexer<Iter>& lexer);
std::expected<Json, ParseError> parse_recurse_arr(Lexer<Iter>& lexer);

std::expected<Json, ParseError> parse_recurse_obj(Lexer<Iter>& lexer) {
    using T = Token;
    Obj obj{};
    Lexeme lexeme = lexer.next();
    if (lexeme.tok == T::BraceClose) { return Json(std::move(obj)); }
    std::string_view key{};
    while (true) {
        if (lexeme.tok != T::Str)
            return mk_err(lexer, "Expected string literal");
        key = lexeme.value;
        lexeme = lexer.next();
        if (lexeme.tok != T::Colon) return mk_err(lexer, "Expected ':'");
        lexer.next();
        auto val = parse_recurse(lexer);
        if (!val) return val;
        obj[std::string(key)] = std::move(*val);
        lexeme = lexer.next();
        if (lexeme.tok == T::Comma) {
            lexeme = lexer.next();
            continue;
        }
        if (lexeme.tok == T::BraceClose) break;
        return mk_err(lexer, "Unexpected token"_static);
    }
    return Json(std::move(obj));
}

std::expected<Json, ParseError> parse_recurse_arr(Lexer<Iter>& lexer) {
    using T = Token;
    Arr arr{};
    Lexeme lexeme = lexer.next();
    if (lexeme.tok == T::BracketClose) { return Json(std::move(arr)); }
    while (true) {
        auto val = parse_recurse(lexer);
        if (!val) return val;
        arr.push_back(std::move(*val));
        lexeme = lexer.next();
        if (lexeme.tok == T::Comma) {
            lexeme = lexer.next();
            continue;
        }
        if (lexeme.tok == T::BracketClose) break;
    }
    return Json(std::move(arr));
}

std::expected<Json, ParseError> parse_recurse(Lexer<Iter>& lexer) {
    using T = Token;
    auto [tok, val] = lexer.curr();
    switch (tok) {
        case T::Null: return Json();
        case T::True: return Json{true};
        case T::False: return Json{false};
        case T::Int: return Json{to_int(val)};
        case T::Float: return Json{to_float(val)};
        case T::Str: return Json{Str(val)};
        case T::Brace: return parse_recurse_obj(lexer);
        case T::Bracket: return parse_recurse_arr(lexer);
        case T::Error: return mk_err(lexer, "Invalid token");
        case T::End: return mk_err(lexer, "Unexpected end of input");
    }
    return mk_err(lexer, "Unexpected token");
}

}  // namespace
std::expected<Json, ParseError> parse(std::string_view s) {
    Lexer<Iter> lexer{s.begin(), s.end()};
    lexer.next();
    return parse_recurse(lexer);
}
}  // namespace cent::sd::json
