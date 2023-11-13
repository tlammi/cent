#pragma once

#include <optional>
#include <string_view>
#include <vector>

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

namespace lex_detail {
constexpr bool is_ws(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
constexpr bool front_eq(std::string_view s, char c) {
    if (s.empty()) return false;
    return s.front() == c;
}
}  // namespace lex_detail

class Lexer {
 public:
    Lexer() = default;
    explicit constexpr Lexer(std::string_view s) noexcept : m_s{s} {}

    [[nodiscard]] constexpr Lexeme current() noexcept { return m_curr; }
    constexpr Lexeme next() noexcept {
        discard_ws();
        m_curr = lex_next();
        if (m_curr.token != Token::Err) m_s.remove_prefix(m_curr.value.size());
        return m_curr;
    }

 private:
    static constexpr std::string_view null_lit = "null";
    static constexpr std::string_view true_lit = "true";
    static constexpr std::string_view false_lit = "false";
    constexpr Lexeme lex_next() const noexcept {
        using enum Token;
        if (m_s.empty()) return {Token::Eof, ""};
        switch (m_s.front()) {
            case '{': return {StartObject, m_s.substr(0, 1)};
            case '}': return {EndObject, m_s.substr(0, 1)};
            case '[': return {StartArray, m_s.substr(0, 1)};
            case ']': return {EndArray, m_s.substr(0, 1)};
            case ',': return {Comma, m_s.substr(0, 1)};
            case ':': return {Colon, m_s.substr(0, 1)};
        }
        if (m_s.starts_with(null_lit))
            return {Null, m_s.substr(0, null_lit.size())};
        if (m_s.starts_with(true_lit))
            return {True, m_s.substr(0, true_lit.size())};
        if (m_s.starts_with(false_lit))
            return {False, m_s.substr(0, false_lit.size())};
        if (m_s.starts_with('"')) { return lex_next_str(); }
        if (m_s.front() == '-' || (m_s.front() >= '0' && m_s.front() <= '9'))
            return lex_next_num();
        return {Err, "Invalid token"};
    }

    constexpr Lexeme lex_next_str() const noexcept {
        size_t i = 1;
        while (true) {
            if (i >= m_s.size()) return {Token::Err, "Non-terminated string"};
            if (m_s[i] == '"') return {Token::Str, m_s.substr(0, i + 1)};
            if (m_s[i] == '\\') ++i;
            ++i;
        }
    }

    constexpr Lexeme lex_next_num() const noexcept {
        std::string_view work = m_s;
        bool is_float = false;
        if (work.starts_with('-')) work.remove_prefix(1);
        if (work.starts_with('0')) {
            work.remove_prefix(1);
        } else if (!work.empty() && work.front() >= '1' &&
                   work.front() <= '9') {
            while (!work.empty() && work.front() >= '0' &&
                   work.front() <= '9') {
                work.remove_prefix(1);
            }
        }
        if (work.starts_with('.')) {
            is_float = true;
            work.remove_prefix(1);
            if (work.empty() && !(work.front() > '0' && work.front() <= '9')) {
                return {Token::Err, "Decimals expected"};
            }
            while (!work.empty() && work.front() >= '0' &&
                   work.front() <= '9') {
                work.remove_prefix(1);
            }
        }
        if (work.starts_with('e') || work.starts_with('E')) {
            is_float = true;
            work.remove_prefix(1);
            if (work.empty()) return {Token::Err, "Exponent expected"};
            if (work.front() == '-' || work.front() == '+')
                work.remove_prefix(1);
            if (work.empty() && !(work.front() >= '0' && work.front() <= '9'))
                return {Token::Err, "Exponent expected"};
            while (!work.empty() && work.front() >= '0' &&
                   work.front() <= '9') {
                work.remove_prefix(1);
            }
        }
        return {is_float ? Token::Float : Token::Int,
                m_s.substr(0, m_s.size() - work.size())};
    }

    constexpr void discard_ws() noexcept {
        while (!m_s.empty() && lex_detail::is_ws(m_s.front()))
            m_s.remove_prefix(1);
    }

    std::string_view m_s{};
    Lexeme m_curr{};
};

inline std::vector<Lexeme> lex_all(std::string_view s) {
    std::vector<Lexeme> lexemes{};
    Lexer l{s};
    while (true) {
        lexemes.emplace_back(l.next());
        if (lexemes.back().token == Token::Eof ||
            lexemes.back().token == Token::Err)
            break;
    }
    return lexemes;
}

}  // namespace cent::json
