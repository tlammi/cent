#include "json_lex.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <ranges>

namespace json = cent::sd::json;
using namespace testing;

std::vector<json::Token> get_tokens(const std::vector<json::Lexeme>& lexemes) {
    std::vector<json::Token> tokens;
    tokens.reserve(lexemes.size());
    for (const auto& l : lexemes) { tokens.push_back(l.tok); }
    return tokens;
}

std::vector<std::string_view> get_values(
    const std::vector<json::Lexeme>& lexemes) {
    std::vector<std::string_view> values;
    values.reserve(lexemes.size());
    for (const auto& l : lexemes) { values.push_back(l.value); }
    return values;
}

TEST(Lex, Empty) {
    auto lexer = json::make_lexer("");
    auto lexemes = lexer.lex_all();
    auto toks = get_tokens(lexemes);
    ASSERT_THAT(toks, ElementsAre(json::Token::End));
}

TEST(Lex, Error) {
    auto lexer = json::make_lexer("a");
    auto lexemes = lexer.lex_all();
    auto toks = get_tokens(lexemes);
    ASSERT_THAT(toks, ElementsAre(json::Token::Error));
}

TEST(Lex, EmptyStr) {
    auto lexer = json::make_lexer("     ");
    auto lexemes = lexer.lex_all();
    auto toks = get_tokens(lexemes);
    ASSERT_THAT(toks, ElementsAre(json::Token::End));
}

TEST(Lex, EmptyObj) {
    auto lexer = json::make_lexer("{}");
    auto lexemes = lexer.lex_all();
    auto toks = get_tokens(lexemes);
    using T = json::Token;
    ASSERT_THAT(toks, ElementsAre(T::Brace, T::BraceClose, T::End));
}

TEST(Lex, Atoms) {
    auto lexer = json::make_lexer("true false null");
    auto lexemes = lexer.lex_all();
    auto toks = get_tokens(lexemes);
    using T = json::Token;
    ASSERT_THAT(toks, ElementsAre(T::True, T::False, T::Null, T::End));
}

TEST(Lex, StrLit) {
    auto lexer = json::make_lexer("\"hello\"");
    auto lexemes = lexer.lex_all();
    auto toks = get_tokens(lexemes);
    auto vals = get_values(lexemes);
    using T = json::Token;
    ASSERT_THAT(toks, ElementsAre(T::Str, T::End));
    ASSERT_THAT(vals, ElementsAre("hello", ""));
}

TEST(Lex, Int) {
    auto lexer = json::make_lexer("123");
    auto lexemes = lexer.lex_all();
    auto toks = get_tokens(lexemes);
    auto vals = get_values(lexemes);
    using T = json::Token;
    ASSERT_THAT(toks, ElementsAre(T::Int, T::End));
    ASSERT_THAT(vals, ElementsAre("123", ""));
}

TEST(Lex, Zero) {
    auto lexer = json::make_lexer("0");
    auto lexemes = lexer.lex_all();
    auto toks = get_tokens(lexemes);
    auto vals = get_values(lexemes);
    using T = json::Token;
    ASSERT_THAT(toks, ElementsAre(T::Int, T::End));
    ASSERT_THAT(vals, ElementsAre("0", ""));
}

TEST(Lex, Float) {
    auto lexer = json::make_lexer("123.456");
    auto lexemes = lexer.lex_all();
    auto toks = get_tokens(lexemes);
    auto vals = get_values(lexemes);
    using T = json::Token;
    ASSERT_EQ(toks.at(0), T::Float) << vals.at(0);
    ASSERT_EQ(toks.at(1), T::End) << vals.at(1);
    ASSERT_THAT(vals, ElementsAre("123.456", ""));
}

TEST(Lex, Exp) {
    auto lexer = json::make_lexer("123.456e789");
    auto lexemes = lexer.lex_all();
    auto toks = get_tokens(lexemes);
    auto vals = get_values(lexemes);
    using T = json::Token;
    ASSERT_EQ(toks.at(0), T::Float) << vals.at(0);
    ASSERT_EQ(toks.at(1), T::End) << vals.at(1);
    ASSERT_THAT(vals, ElementsAre("123.456e789", ""));
}

TEST(Lex, BackToBack) {
    auto lexer = json::make_lexer("1,2,3\"foo\",[]");
    auto lexemes = lexer.lex_all();
    auto toks = get_tokens(lexemes);
    auto vals = get_values(lexemes);
    using T = json::Token;
    ASSERT_THAT(toks,
                ElementsAre(T::Int, T::Comma, T::Int, T::Comma, T::Int, T::Str,
                            T::Comma, T::Bracket, T::BracketClose, T::End));
}

TEST(Lex, Position) {
    std::vector<json::Position> positions{};
    std::vector<json::Position> expected{{1, 1},  {2, 1}, {2, 2},
                                         {2, 3},  {2, 4}, {2, 5},
                                         {2, 10}, {3, 1}, {3, 2}};

    auto lexer = json::make_lexer("1\n,2,3\"foo\",\n[]");
    while (true) {
        auto l = lexer.next();
        ASSERT_NE(l.tok, json::Token::Error);
        if (l.tok == json::Token::End) break;
        positions.push_back(lexer.pos());
    }
    size_t i = 0;
    ASSERT_EQ(positions.size(), expected.size());
    for (const auto [l, r] : std::ranges::views::zip(positions, expected)) {
        ASSERT_EQ(l, r) << "index: " << i;
        ++i;
    }
}
