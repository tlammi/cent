#pragma once

#include <cent/dt/result.hpp>
#include <cent/json/json.hpp>
#include <cent/json/lex.hpp>
#include <cent/util/str.hpp>
#include <string_view>

namespace cent::json {
namespace parse_detail {

std::string_view rm_quotes(std::string_view s) {
    s.remove_prefix(1);
    s.remove_suffix(1);
    return s;
}

Result<Json> parse_recurse(Lexer& l);

Result<Json> parse_recurse_obj(Lexer& l) {
    static constexpr auto invalid = std::errc::invalid_argument;
    using enum Token;
    Json jsn{Obj()};
    auto& obj = jsn.as_obj();
    auto lexeme = l.next();
    if (lexeme.token == EndObject) {
        l.next();
        return jsn;
    }
    while (true) {
        auto lexeme = l.current();
        if (lexeme.token != Str) return {invalid};
        auto key = rm_quotes(lexeme.value);
        lexeme = l.next();
        if (lexeme.token != Colon) return {invalid};
        l.next();
        auto val = parse_recurse(l);
        if (!val) return val;
        obj[std::string(key)] = std::move(*val);
        lexeme = l.next();
        if (lexeme.token == EndObject) break;
        if (lexeme.token != Comma) { return {invalid}; }
        l.next();
    }
    return jsn;
}

Result<Json> parse_recurse_arr(Lexer& l) {
    using enum Token;
    auto arr = Arr();
    auto lexeme = l.next();
    if (lexeme.token == EndArray) {
        l.next();
        return Json(std::move(arr));
    }
    while (true) {
        auto val = parse_recurse(l);
        if (!val) return val;
        arr.push_back(std::move(*val));
        lexeme = l.next();
        if (lexeme.token == EndArray) { return Json(std::move(arr)); }
        if (lexeme.token != Comma) return {std::errc::invalid_argument};
    }
}

Result<Json> parse_recurse(Lexer& l) {
    using enum Token;
    static constexpr auto invalid = std::errc::invalid_argument;
    auto [tok, val] = l.current();
    switch (tok) {
        case Err: return {invalid};
        case Eof: return {invalid};
        case Null: return Json();
        case True: return Json(true);
        case False: return Json(false);
        case Int: return Json(parse_int<::cent::json::Int>(val).unwrap());
        case Float: return Json(parse_float<::cent::json::Float>(val).unwrap());
        case Str: return Json(std::string(parse_detail::rm_quotes(val)));
        case StartObject: return parse_recurse_obj(l);
        case StartArray: return parse_recurse_arr(l);
        case EndObject:
        case EndArray:
        case Comma:
        case Colon: {
        }
    }
    return {invalid};
}
}  // namespace parse_detail

Result<Json> parse(std::string_view s) {
    auto lexer = Lexer(s);
    lexer.next();
    return parse_detail::parse_recurse(lexer);
}

}  // namespace cent::json
