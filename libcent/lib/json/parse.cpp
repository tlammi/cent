
#include <cent/json/parse.hpp>

namespace cent::json {
namespace {
namespace parse_detail {

std::string_view rm_quotes(std::string_view s) {
    s.remove_prefix(1);
    s.remove_suffix(1);
    return s;
}

using LexemeOrJson = std::variant<Lexeme, Json>;

Result<Json> squash_tail_obj(std::vector<LexemeOrJson>& stack) {
    auto out = Json(Obj());
    auto& obj = out.as_obj();
    using enum Token;
    if (stack.empty()) return Errno::Einval;
    auto* maybe_begin = std::get_if<Lexeme>(&stack.back());
    if (maybe_begin && maybe_begin->token == StartObject) {
        stack.pop_back();
        return out;
    }
    while (true) {
        if (stack.size() < 4) return Errno::Einval;
        auto* val = std::get_if<Json>(&stack.at(stack.size() - 1));
        auto* colon = std::get_if<Lexeme>(&stack.at(stack.size() - 2));
        auto* key = std::get_if<Json>(&stack.at(stack.size() - 3));
        auto* comma_or_begin = std::get_if<Lexeme>(&stack.at(stack.size() - 4));
        if (!(val && colon && key && comma_or_begin)) return Errno::Einval;
        if (colon->token != Colon) return Errno::Einval;

        if (!key->is_str()) return Errno::Einval;
        obj[std::move(key->as_str())] = std::move(*val);
        auto tok = comma_or_begin->token;
        stack.pop_back();
        stack.pop_back();
        stack.pop_back();
        stack.pop_back();
        if (tok == StartObject) return out;
        if (tok == Comma) continue;
        return Errno::Einval;
    }
}

Result<Json> squash_tail_arr(std::vector<LexemeOrJson>& stack) {
    auto out = Json(Arr());
    auto& arr = out.as_arr();
    using enum Token;
    std::vector<Json> tmp{};
    if (stack.empty()) return Errno::Einval;
    auto* maybe_begin = std::get_if<Lexeme>(&stack.back());
    if (maybe_begin && maybe_begin->token == StartArray) {
        stack.pop_back();
        return out;
    }

    while (true) {
        if (stack.size() < 2) return Errno::Einval;
        auto* val = std::get_if<Json>(&stack.at(stack.size() - 1));
        auto* comma_or_begin = std::get_if<Lexeme>(&stack.at(stack.size() - 2));
        if (!(val && comma_or_begin)) return Errno::Einval;
        tmp.push_back(std::move(*val));
        auto tok = comma_or_begin->token;
        stack.pop_back();
        stack.pop_back();
        if (tok == StartArray) {
            // TODO: better implementation for this
            for (auto& v : tmp) { arr.push_back(std::move(v)); }
            return out;
        }
        if (tok == Comma) continue;
        return Errno::Einval;
    }
}

Result<Json> parse_iterate(Lexer& l) {
    std::vector<LexemeOrJson> stack{};
    using enum Token;
    while (true) {
        auto lexeme = l.next();
        switch (lexeme.token) {
            case Eof: {
                if (stack.size() != 1) return Errno::Einval;
                if (!std::holds_alternative<Json>(stack.front()))
                    return Errno::Einval;
                return std::get<Json>(std::move(stack.front()));
            }
            case Err: return Errno::Einval;
            case Null: {
                stack.push_back(Json());
                break;
            }
            case True: {
                stack.push_back(Json(true));
                break;
            }
            case False: {
                stack.push_back(Json(false));
                break;
            }
            case Int: {
                stack.push_back(
                    Json(parse_int<::cent::json::Int>(lexeme.value).unwrap()));
                break;
            }
            case Float: {
                auto res = parse_float<::cent::json::Float>(lexeme.value);
                if (!res) return Result<Json>(res.error());
                stack.push_back(Json(std::move(*res)));
                break;
            }
            case Str: {
                stack.push_back(
                    Json(std::string(parse_detail::rm_quotes(lexeme.value))));
                break;
            }
            case Colon:
            case Comma:
            case StartObject:
            case StartArray: {
                stack.push_back(lexeme);
                break;
            }
            case EndObject: {
                auto res = squash_tail_obj(stack);
                if (!res) return res;
                stack.push_back(std::move(*res));
                break;
            }
            case EndArray: {
                auto res = squash_tail_arr(stack);
                if (!res) return res;
                stack.push_back(std::move(*res));
                break;
            }
        }
    }
}
}  // namespace parse_detail
}  // namespace

Result<Json> parse(std::string_view s) {
    auto lexer = Lexer(s);
    return parse_detail::parse_iterate(lexer);
}
}  // namespace cent::json
