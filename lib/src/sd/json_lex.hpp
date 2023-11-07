#pragma once

#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

namespace cent::sd::json {

struct Position {
    size_t line;
    size_t column;

    constexpr bool operator==(Position other) const noexcept {
        return line == other.line && column == other.column;
    }
};

constexpr bool is_space(char c) noexcept {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

template <class Iter, class Sentinel = Iter>
class Stream {
 public:
    constexpr Stream(Iter iter, Sentinel end) noexcept
        : m_iter(iter), m_end(end) {}

    constexpr operator bool() const noexcept { return !empty(); }
    constexpr bool empty() const noexcept { return m_iter == m_end; }

    constexpr auto peek() const noexcept { return *m_iter; }
    constexpr void pop(size_t count = 1) noexcept {
        while (count--) {
            if (peek() == '\n') {
                std::cerr << "line switch\n";
                ++m_pos.line;
                m_pos.column = 1;
            } else {
                ++m_pos.column;
            }
            ++m_iter;
        }
    }

    constexpr std::string_view prefix(size_t n) const noexcept {
        return std::string_view{m_iter, m_iter + n};
    }

    constexpr bool starts_with(std::string_view s) const noexcept {
        return static_cast<std::string_view::size_type>(m_end - m_iter) >=
                   s.size() &&
               std::equal(s.begin(), s.end(), m_iter);
    }

    constexpr auto operator[](size_t n) const noexcept { return *(m_iter + n); }

    constexpr size_t size() const noexcept { return m_end - m_iter; }

    constexpr void strip_left() noexcept {
        while (!empty()) {
            if (is_space(peek()))
                pop();
            else
                break;
        }
    }

    [[nodiscard]] constexpr Position pos() const noexcept { return m_pos; }

 private:
    Iter m_iter;
    Sentinel m_end;
    Position m_pos{1, 1};
};

enum class Token : std::uint8_t {
    Null,
    False,
    True,
    Int,
    Float,
    Str,
    Brace,
    BraceClose,
    Bracket,
    BracketClose,
    Colon,
    Comma,
    End,
    Error,
};

struct Lexeme {
    Token tok;
    std::string_view value{};
};

template <class Iter, class Sentinel = Iter>
class Lexer {
 public:
    constexpr Lexer(Iter iter, Sentinel end) noexcept : m_s{iter, end} {}

    constexpr std::vector<Lexeme> lex_all() {
        std::vector<Lexeme> out{};
        while (true) {
            out.emplace_back(next());
            if (out.back().tok == Token::End || out.back().tok == Token::Error)
                break;
        }
        return out;
    }

    constexpr Lexeme next() noexcept {
        m_s.strip_left();
        m_pos = m_s.pos();
        if (!m_s) return m_curr = {Token::End, {}};
        using T = Token;
        switch (m_s.peek()) {
            case '{': m_s.pop(); return m_curr = {T::Brace};
            case '}': m_s.pop(); return m_curr = {T::BraceClose};
            case '[': m_s.pop(); return m_curr = {T::Bracket};
            case ']': m_s.pop(); return m_curr = {T::BracketClose};
            case ',': m_s.pop(); return m_curr = {T::Comma};
            case ':': m_s.pop(); return m_curr = {T::Colon};
            case '"': m_s.pop(); return m_curr = lex_str();
            default: break;
        }
        if ((m_s.peek() >= '0' && m_s.peek() <= '9') || m_s.peek() == '-')
            return m_curr = lex_num();

        if (m_s.starts_with("null")) {
            m_s.pop(4);
            return m_curr = {T::Null, {}};
        }
        if (m_s.starts_with("true")) {
            m_s.pop(4);
            return m_curr = {T::True, {}};
        }
        if (m_s.starts_with("false")) {
            m_s.pop(5);  // NOLINT
            return m_curr = {T::False, {}};
        }
        return m_curr = {T::Error, {}};
    }

    [[nodiscard]] constexpr Lexeme curr() const noexcept { return m_curr; }

    [[nodiscard]] constexpr Position pos() const noexcept { return m_pos; }

 private:
    constexpr Lexeme lex_str() noexcept {
        size_t i = 0;
        for (; i < m_s.size(); ++i) {
            if (m_s[i] == '\\')
                ++i;
            else if (m_s[i] == '"')
                break;
        }
        auto val = m_s.prefix(i);
        m_s.pop(i + 1);
        return {Token::Str, val};
    }

    constexpr Lexeme lex_num() noexcept {
        size_t i = 0;
        bool is_float = false;
        if (m_s.peek() == '-') ++i;
        if (i >= m_s.size()) return {Token::Error, {}};
        i = walk_integer(i);
        if (i >= m_s.size()) {
            auto val = m_s.prefix(i);
            m_s.pop(i);
            return {Token::Int, val};
        }
        if (m_s[i] == '.') {
            is_float = true;
            ++i;
            if (i >= m_s.size()) return {Token::Error, {}};
            i = walk_integer(i);
            if (i >= m_s.size()) {
                auto val = m_s.prefix(i);
                m_s.pop(i);
                return {Token::Float, val};
            }
        }
        if (m_s[i] == 'e' || m_s[i] == 'E') {
            is_float = true;
            ++i;
            if (i >= m_s.size()) return {Token::Error, {}};
            if (m_s.peek() == '-' || m_s.peek() == '+') {
                ++i;
                if (i >= m_s.size()) return {Token::Error, {}};
            }
            i = walk_integer(i);
        }
        auto val = m_s.prefix(i);
        m_s.pop(i);
        return {is_float ? Token::Float : Token::Int, val};
    }

    constexpr size_t walk_integer(size_t i) const noexcept {
        for (; i < m_s.size(); ++i) {
            if (m_s[i] < '0' || m_s[i] > '9') break;
        }
        return i;
    }

    Stream<Iter, Sentinel> m_s;
    Lexeme m_curr{};
    Position m_pos{1, 1};
};

constexpr auto make_lexer(std::string_view s) {
    return Lexer{s.begin(), s.end()};
}

}  // namespace cent::sd::json
