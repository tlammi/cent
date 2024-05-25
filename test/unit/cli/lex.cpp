#include <gtest/gtest.h>

#include <cent/cli/lexer.hpp>

template <class... Ts>
std::vector<const char*> mk_args(Ts&&... ts) {
    return {std::forward<Ts>(ts)...};
}

using cent::cli::Arg;
using cent::cli::ArgType;

std::vector<Arg> lex_all(const std::vector<const char*> args) {
    auto lexer = cent::cli::make_lexer(args.size(), args.data());
    std::vector<Arg> out{};
    while (auto arg = lexer()) { out.push_back(arg); }
    return out;
}

std::string to_string(const std::vector<Arg>& v) {
    std::string out;
    for (const auto& arg : v) {
        out += arg.value;
        out += ", ";
    }
    return out;
}

TEST(Lex, Empty) {
    auto args = mk_args();
    auto lexemes = lex_all(args);
    ASSERT_TRUE(lexemes.empty());
}

TEST(Lex, Short) {
    auto args = mk_args("-a");
    auto lexemes = lex_all(args);
    ASSERT_EQ(lexemes.size(), 1);
    ASSERT_EQ(lexemes[0].type, ArgType::Short);
    ASSERT_EQ(lexemes[0].value, "a");
}

TEST(Lex, Long) {
    auto args = mk_args("--long");
    auto lexemes = lex_all(args);
    ASSERT_EQ(lexemes.size(), 1);
    ASSERT_EQ(lexemes[0].type, ArgType::Long);
    ASSERT_EQ(lexemes[0].value, "long");
}

TEST(Lex, Val) {
    auto args = mk_args("val");
    auto lexemes = lex_all(args);
    ASSERT_EQ(lexemes.size(), 1);
    ASSERT_EQ(lexemes[0].type, ArgType::Value);
    ASSERT_EQ(lexemes[0].value, "val");
}

TEST(Lex, MultipleShorts) {
    auto args = mk_args("-ab", "-c", "-de");
    auto lexemes = lex_all(args);
    ASSERT_EQ(lexemes.size(), 5);
    ASSERT_EQ(lexemes.at(0).type, ArgType::Short);
    ASSERT_EQ(lexemes.at(0).value, "a");
    ASSERT_EQ(lexemes.at(1).type, ArgType::Short);
    ASSERT_EQ(lexemes.at(1).value, "b");
    ASSERT_EQ(lexemes.at(2).type, ArgType::Short);
    ASSERT_EQ(lexemes.at(2).value, "c");
    ASSERT_EQ(lexemes.at(3).type, ArgType::Short);
    ASSERT_EQ(lexemes.at(3).value, "d");
    ASSERT_EQ(lexemes.at(4).type, ArgType::Short);
    ASSERT_EQ(lexemes.at(4).value, "e");
}

TEST(Lex, ShortWithEq) {
    auto args = mk_args("-a=b");
    auto lexemes = lex_all(args);
    ASSERT_EQ(lexemes.size(), 2);
    ASSERT_EQ(lexemes.at(0).type, ArgType::Short);
    ASSERT_EQ(lexemes.at(0).value, "a");
    ASSERT_EQ(lexemes.at(1).type, ArgType::Value);
    ASSERT_EQ(lexemes.at(1).value, "b");
}

TEST(Lex, MultipleShortWithEq) {
    auto args = mk_args("-abc=def");

    auto lexemes = lex_all(args);
    ASSERT_EQ(lexemes.size(), 4);
    ASSERT_EQ(lexemes.at(0).type, ArgType::Short);
    ASSERT_EQ(lexemes.at(0).value, "a");
    ASSERT_EQ(lexemes.at(1).type, ArgType::Short);
    ASSERT_EQ(lexemes.at(1).value, "b");
    ASSERT_EQ(lexemes.at(2).type, ArgType::Short);
    ASSERT_EQ(lexemes.at(2).value, "c");
    ASSERT_EQ(lexemes.at(3).type, ArgType::Value);
    ASSERT_EQ(lexemes.at(3).value, "def");
}

TEST(Lex, LongWithEq) {
    auto args = mk_args("--long=eq");
    auto lexemes = lex_all(args);
    ASSERT_EQ(lexemes.size(), 2);
    ASSERT_EQ(lexemes.at(0).type, ArgType::Long);
    ASSERT_EQ(lexemes.at(0).value, "long");
    ASSERT_EQ(lexemes.at(1).type, ArgType::Value);
    ASSERT_EQ(lexemes.at(1).value, "eq");
}

TEST(Lex, ValWithEq) {
    auto args = mk_args("-s==", "--long1", "=", "--long2==");
    auto lexemes = lex_all(args);
    ASSERT_EQ(lexemes.size(), 6) << to_string(lexemes);
    ASSERT_EQ(lexemes.at(0).type, ArgType::Short);
    ASSERT_EQ(lexemes.at(0).value, "s");
    ASSERT_EQ(lexemes.at(1).type, ArgType::Value);
    ASSERT_EQ(lexemes.at(1).value, "=");
    ASSERT_EQ(lexemes.at(2).type, ArgType::Long);
    ASSERT_EQ(lexemes.at(2).value, "long1");
    ASSERT_EQ(lexemes.at(3).type, ArgType::Value);
    ASSERT_EQ(lexemes.at(3).value, "=");
    ASSERT_EQ(lexemes.at(4).type, ArgType::Long);
    ASSERT_EQ(lexemes.at(4).value, "long2");
    ASSERT_EQ(lexemes.at(5).type, ArgType::Value);
    ASSERT_EQ(lexemes.at(5).value, "=");
}
