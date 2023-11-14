#include <gtest/gtest.h>

#include <cent/json/lex.hpp>

using cent::json::Lexeme;
using cent::json::Lexer;
using cent::json::Token;

TEST(Lexer, Empty) {
    Lexer lexer{};
    auto lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::Eof);
    ASSERT_EQ(lexer.current().token, Token::Eof);
}

TEST(Lexer, WhiteSpace) {
    Lexer lexer{" \t\n\r"};
    auto lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::Eof);
    ASSERT_EQ(lexer.current().token, Token::Eof);
}

TEST(Lexer, Null) {
    Lexer lexer{"null"};
    auto lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::Null);
    ASSERT_EQ(lexer.current().token, Token::Null);
}

TEST(Lexer, True) {
    Lexer lexer{"true"};
    auto lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::True);
    ASSERT_EQ(lexer.current().token, Token::True);
}

TEST(Lexer, False) {
    Lexer lexer{"false"};
    auto lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::False);
    ASSERT_EQ(lexer.current().token, Token::False);
}

TEST(Lexer, Int) {
    Lexer lexer{"42"};
    auto lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::Int);
    ASSERT_EQ(lexeme.value, "42");
    ASSERT_EQ(lexer.current().token, Token::Int);
}

TEST(Lexer, Float) {
    Lexer lexer{"42.0"};
    auto lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::Float) << lexeme.value;
    ASSERT_EQ(lexeme.value, "42.0");
    ASSERT_EQ(lexer.current().token, Token::Float);
}

TEST(Lexer, Exp) {
    Lexer lexer{"42e3"};
    auto lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::Float);
    ASSERT_EQ(lexeme.value, "42e3");
    ASSERT_EQ(lexer.current().token, Token::Float);
}

TEST(Lexer, FloatExp) {
    Lexer lexer{"42.0e3"};
    auto lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::Float);
    ASSERT_EQ(lexeme.value, "42.0e3");
    ASSERT_EQ(lexer.current().token, Token::Float);
}

TEST(Lexer, FloatExp2) {
    Lexer lexer{"123.456e-789"};
    auto lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::Float);
    ASSERT_EQ(lexeme.value, "123.456e-789");
    ASSERT_EQ(lexer.current().token, Token::Float);
}

TEST(Lexer, Str) {
    Lexer lexer{R"("hello")"};
    auto lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::Str);
    ASSERT_EQ(lexeme.value, R"("hello")");
    ASSERT_EQ(lexer.current().token, Token::Str);
}

TEST(Lexer, StrNonTerminated) {
    Lexer lexer{R"("hello)"};
    auto lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::Err);
}

TEST(Lexer, EmptyObj) {
    Lexer lexer{"{  }"};
    auto lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::StartObject);
    ASSERT_EQ(lexer.current().token, Token::StartObject);
    lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::EndObject);
    ASSERT_EQ(lexer.current().token, Token::EndObject);
    lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::Eof);
    ASSERT_EQ(lexer.current().token, Token::Eof);
}

TEST(Lexer, EmptyArr) {
    Lexer lexer{"[ ]"};
    auto lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::StartArray);
    ASSERT_EQ(lexer.current().token, Token::StartArray);
    lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::EndArray);
    ASSERT_EQ(lexer.current().token, Token::EndArray);
    lexeme = lexer.next();
    ASSERT_EQ(lexeme.token, Token::Eof);
    ASSERT_EQ(lexer.current().token, Token::Eof);
}

