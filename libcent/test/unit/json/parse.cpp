#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cent/json/parse.hpp>

using cent::json::lex_all;
using cent::json::lex_next;
using cent::json::Lexeme;
using cent::json::Token;

std::vector<Token> get_tokens(const std::vector<Lexeme>& lexemes) {
    std::vector<Token> out{};
    out.reserve(lexemes.size());
    for (const auto& l : lexemes) { out.emplace_back(l.token); }
    return out;
}

std::vector<std::string_view> get_values(const std::vector<Lexeme>& lexemes) {
    std::vector<std::string_view> out{};
    out.reserve(lexemes.size());
    for (const auto& l : lexemes) { out.emplace_back(l.value); }
    return out;
}

TEST(Lex, Empty) {
    auto lexemes = lex_all("");
    auto toks = get_tokens(lexemes);
    ASSERT_EQ(toks.size(), 1);
    ASSERT_EQ(toks[0], Token::Eof);
}

TEST(Lex, Int) {
    auto lexemes = lex_all("123");
    auto toks = get_tokens(lexemes);
    ASSERT_EQ(toks.size(), 2);
    ASSERT_EQ(toks[0], Token::Int);
    ASSERT_EQ(toks[1], Token::Eof);
}

TEST(Lex, Float) {
    auto lexemes = lex_all("123.456");
    auto toks = get_tokens(lexemes);
    ASSERT_EQ(toks.size(), 2);
    ASSERT_EQ(toks[0], Token::Float);
    ASSERT_EQ(toks[1], Token::Eof);
}

TEST(Lex, Str) {
    auto lexemes = lex_all("\"hello\"");
    auto toks = get_tokens(lexemes);
    {
        auto [tok, val] = lexemes.at(0);
        ASSERT_EQ(tok, Token::Str);
        ASSERT_EQ(val, R"("hello")");
    }
    ASSERT_EQ(toks.size(), 2);
    ASSERT_EQ(toks[1], Token::Eof);
}

TEST(Lex, StrNonTerminated) {
    auto lexemes = lex_all("\"hello");
    auto toks = get_tokens(lexemes);
    ASSERT_EQ(toks.size(), 1);
    ASSERT_EQ(toks[0], Token::Err);
}

TEST(Lex, StrEscapedQuote) {
    auto lexemes = lex_all(R"("hello\"")");
    {
        auto [tok, val] = lexemes.at(0);
        ASSERT_EQ(tok, Token::Str);
        ASSERT_EQ(val, R"("hello\"")");
    }
    ASSERT_EQ(lexemes.at(1).token, Token::Eof);
}

TEST(Lex, Exp) {
    auto lexemes = lex_all("123.456e789");
    auto toks = get_tokens(lexemes);
    auto vals = get_values(lexemes);
    ASSERT_EQ(toks.size(), 2);
    ASSERT_EQ(toks[0], Token::Float);
    ASSERT_EQ(vals[0], "123.456e789");
    ASSERT_EQ(toks[1], Token::Eof);
}

TEST(Lex, ExpPlus) {
    auto lexemes = lex_all("123.456e+789");
    ASSERT_EQ(lexemes.size(), 2);
    {
        auto [tok, val] = lexemes.at(0);
        ASSERT_EQ(tok, Token::Float);
        ASSERT_EQ(val, "123.456e+789");
    }
    ASSERT_EQ(lexemes.at(1).token, Token::Eof);
}

TEST(Lex, ExpMinus) {
    auto lexemes = lex_all("123.456e-789");
    auto toks = get_tokens(lexemes);
    ASSERT_EQ(toks.size(), 2);
    ASSERT_EQ(toks[0], Token::Float);
    ASSERT_EQ(toks[1], Token::Eof);
}

TEST(Lex, True) {
    auto lexemes = lex_all("true");
    auto toks = get_tokens(lexemes);
    ASSERT_EQ(toks.size(), 2);
    ASSERT_EQ(toks[0], Token::True);
    ASSERT_EQ(toks[1], Token::Eof);
}

TEST(Lex, False) {
    auto lexemes = lex_all("false");
    auto toks = get_tokens(lexemes);
    ASSERT_EQ(toks.size(), 2);
    ASSERT_EQ(toks[0], Token::False);
    ASSERT_EQ(toks[1], Token::Eof);
}

TEST(Lex, Null) {
    auto lexemes = lex_all("null");
    auto toks = get_tokens(lexemes);
    ASSERT_EQ(toks.size(), 2);
    ASSERT_EQ(toks[0], Token::Null);
    ASSERT_EQ(toks[1], Token::Eof);
}

TEST(Lex, ArrEmpty) {
    auto lexemes = lex_all("[ ]");
    auto toks = get_tokens(lexemes);
    ASSERT_EQ(toks.size(), 3);
    ASSERT_EQ(toks[0], Token::StartArray);
    ASSERT_EQ(toks[1], Token::EndArray);
    ASSERT_EQ(toks[2], Token::Eof);
}
