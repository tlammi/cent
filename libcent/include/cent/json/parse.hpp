#pragma once

#include <cent/dt/result.hpp>
#include <cent/json/json.hpp>
#include <cent/util/str.hpp>
#include <string_view>

namespace cent::json {

enum class Token {
    Err,
    StartObject,
    EndObject,
    StartArray,
    EndArray,
    Str,
    Int,
    Float,
    True,
    False,
    Null,
    Comma,
    Colon,
    Eof,
};

struct Lexeme {
    Token token;
    std::string_view value;
};

class Lexer {
 public:
 private:
};

constexpr std::string_view split_prefix(std::string_view& s, size_t count) {
    auto prefix = s.substr(0, count);
    s.remove_prefix(count);
    return prefix;
}

namespace detail {

constexpr bool is_ws(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

constexpr bool front_eq(std::string_view s, char c) {
    if (s.empty()) return false;
    return s.front() == c;
}

constexpr void discard_ws(std::string_view& s) {
    while (!s.empty() && is_ws(s.front())) s.remove_prefix(1);
}

constexpr Lexeme lex_str(std::string_view& s) {
    for (size_t i = 1; i < s.size(); ++i) {
        if (s[i] == '"') return {Token::Str, split_prefix(s, i + 1)};
        if (s[i] == '\\') { ++i; }
    }
    return {Token::Err, "Non-terminated string"};
}

constexpr void walk_minus(std::string_view& s) {
    if (s.starts_with('-')) s.remove_prefix(1);
}

constexpr void walk_integral(std::string_view& s) {
    if (s.starts_with('0'))
        s.remove_prefix(1);
    else {
        while (!s.empty() && s.front() >= '0' && s.front() <= '9')
            s.remove_prefix(1);
    }
}

constexpr void walk_decimal(std::string_view& s) {
    while (!s.empty() && s.front() >= '0' && s.front() <= '9')
        s.remove_prefix(1);
}

constexpr Lexeme lex_num(std::string_view& s) {
    bool is_float = false;
    if (s.empty()) return {Token::Err, "Empty number"};
    auto cpy = s;
    walk_minus(cpy);
    walk_integral(cpy);
    if (front_eq(cpy, '.')) {
        is_float = true;
        cpy.remove_prefix(1);
        if (cpy.empty()) return {Token::Err, "Invalid number"};
        walk_decimal(cpy);
    }
    if (front_eq(cpy, 'e') || front_eq(cpy, 'E')) {
        is_float = true;
        cpy.remove_prefix(1);
        if (front_eq(cpy, '-') || front_eq(cpy, '+')) cpy.remove_prefix(1);
        if (cpy.empty()) return {Token::Err, "Invalid exponent"};
        walk_decimal(cpy);
    }
    return {is_float ? Token::Float : Token::Int,
            split_prefix(s, s.size() - cpy.size())};
}

}  // namespace detail

constexpr Lexeme lex_next(std::string_view& s) {
    detail::discard_ws(s);
    if (s.empty()) return {Token::Eof, ""};
    switch (s.front()) {
        case '{': {
            return {Token::StartObject, split_prefix(s, 1)};
        }
        case '}': {
            return {Token::EndObject, split_prefix(s, 1)};
        }
        case '[': {
            return {Token::StartArray, split_prefix(s, 1)};
        }
        case ']': {
            return {Token::EndArray, split_prefix(s, 1)};
        }
        case ',': {
            return {Token::Comma, split_prefix(s, 1)};
        }
        case ':': {
            return {Token::Colon, split_prefix(s, 1)};
        }
    }
    constexpr std::string_view true_literal = "true";
    constexpr std::string_view false_literal = "false";
    constexpr std::string_view null_literal = "null";
    if (s.starts_with(true_literal))
        return {Token::True, split_prefix(s, true_literal.size())};
    if (s.starts_with(false_literal))
        return {Token::False, split_prefix(s, false_literal.size())};
    if (s.starts_with(null_literal))
        return {Token::Null, split_prefix(s, null_literal.size())};
    if (s.starts_with('"')) return detail::lex_str(s);
    if (!s.empty() && s.front() >= '0' && s.front() <= '9')
        return detail::lex_num(s);
    return {Token::Err, "Unknown token"};
}

std::vector<Lexeme> lex_all(std::string_view s) {
    std::vector<Lexeme> lexemes{};
    while (true) {
        lexemes.emplace_back(lex_next(s));
        auto tok = lexemes.back().token;
        if (tok == Token::Eof || tok == Token::Err) break;
    }
    return lexemes;
}

Result<Json> parse_recurse(std::string_view& s) {
    static constexpr auto invalid = std::errc::invalid_argument;
    using enum Token;
    auto [tok, val] = lex_next(s);
    switch (tok) {
        case Err: return {invalid};
        case Eof: return {invalid};
        case Null: return Json();
        case True: return Json(true);
        case False: return Json(false);
        case Int: return Json(parse_int<::cent::json::Int>(val).unwrap());
        case Float: return Json(parse_float<::cent::json::Float>(val).unwrap());
        case Str: return Json(std::string(val));
        case StartObject: {
            Json obj{Obj{}};
            while (true) {
                auto lexeme = lex_next(s);
                if (lexeme.token != Str) return {invalid};
                auto key = lexeme.value;
                lexeme = lex_next(s);
                if (lexeme.token != Colon) return {invalid};
                auto val = parse_recurse(s);
                if (!val) return val.errc();
                obj.as_obj()[std::string(key)] = std::move(*val);
                lexeme = lex_next(s);
                if (lexeme.token == EndObject) return obj;
                if (lexeme.token != Comma) return {invalid};
            }
        }
        case StartArray: {
            Json arr{Arr{}};
            while (true) {
                auto lexeme = lex_next(s);
                if (lexeme.token == Eof || lexeme.token == Err)
                    return {invalid};
                if (lexeme.token == EndArray) return arr;
            }
        }
        case EndObject:
        case EndArray:
        case Comma:
        case Colon: return {std::errc::invalid_argument};
    }
}

Result<Json> parse(std::string_view s);

Result<Json> parse(std::string_view s) { return parse_recurse(s); }

}  // namespace cent::json
