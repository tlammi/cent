#pragma once

#include <sqlite3.h>

#include <cassert>
#include <cent/c_str.hpp>
#include <cent/core/bitmask.hpp>
#include <cent/error.hpp>
#include <cstdint>
#include <ranges>
#include <vector>

namespace cent::strg::sqlite {
namespace detail {
// Specialized in sqlite.cpp
template <class T>
T column(sqlite3_stmt* stmt, int col);

template <class... Ts, int... Is>
std::tuple<Ts...> columns(sqlite3_stmt* stmt,
                          std::integer_sequence<int, Is...>) {
    return std::tuple<Ts...>{column<Ts>(stmt, Is)...};
}

template <class... Ts>
std::tuple<Ts...> columns(sqlite3_stmt* stmt) {
    return columns<Ts...>(stmt,
                          std::make_integer_sequence<int, sizeof...(Ts)>());
}

bool step_query(sqlite3_stmt* stmt);

void bind(sqlite3_stmt* stmt, int idx, int64_t val);
void bind(sqlite3_stmt* stmt, int idx, std::string_view val);

template <int Idx, class T, class... Ts>
void bind_recurse(sqlite3_stmt* stmt, T&& t, Ts&&... ts) {
    bind(stmt, Idx, std::forward<T>(t));
    if constexpr (sizeof...(Ts)) {
        bind_recurse<Idx + 1>(stmt, std::forward<Ts>(ts)...);
    }
}

}  // namespace detail

enum class OpenFlags : std::uint8_t {
    ReadOnly = 0x01,
    ReadWrite = 0x02,
    Create = 0x04,
};

class Connection {
 public:
    constexpr Connection() noexcept = default;
    Connection(CStr str, BitMask<OpenFlags> flags);

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    constexpr Connection(Connection&& other) noexcept
        : m_handle(std::exchange(other.m_handle, nullptr)) {}

    constexpr Connection& operator=(Connection&& other) noexcept {
        std::destroy_at(this);
        std::construct_at(this, std::move(other));
        return *this;
    }

    ~Connection();

    sqlite3* raw() const noexcept { return m_handle; }

 private:
    sqlite3* m_handle{};
};

template <class... Ts>
class Query {
 public:
    constexpr explicit Query(sqlite3_stmt* s) noexcept : m_s(s) {}

    class iterator {
     public:
        using value_type = std::tuple<Ts...>;
        using difference_type = std::ptrdiff_t;

        explicit iterator(sqlite3_stmt* s) : m_s(s) {}

        template <class S>
        decltype(auto) operator*(this S&& s) noexcept {
            return std::forward<S>(s).m_v;
        }

        iterator& operator++() {
            auto data = detail::step_query(m_s);
            if (!data)
                m_s = nullptr;
            else
                m_v = detail::columns<Ts...>(m_s);
            return *this;
        }

        iterator& operator++(int) {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(std::default_sentinel_t /*tag*/) const noexcept {
            return !m_s;
        }

     private:
        sqlite3_stmt* m_s;
        value_type m_v{};
    };

    auto begin() {
        auto it = iterator(m_s);
        ++it;
        return it;
    }

    constexpr std::default_sentinel_t end() const noexcept { return {}; }

 private:
    sqlite3_stmt* m_s;
};

class Stmt {
 public:
    constexpr Stmt() noexcept = default;
    Stmt(Connection& c, std::string_view stmt);

    Stmt(const Stmt&) = delete;
    Stmt& operator=(const Stmt&) = delete;

    constexpr Stmt(Stmt&& other) noexcept
        : m_s(std::exchange(other.m_s, nullptr)) {}

    constexpr Stmt& operator=(Stmt&& other) noexcept {
        std::destroy_at(this);
        std::construct_at(this, std::move(other));
        return *this;
    }

    void execute();

    template <class... Ts>
    auto query() {
        return Query<Ts...>(m_s);
    }

    template <class... Ts>
    auto& bind(Ts&&... ts) {
        detail::bind_recurse<1>(m_s, std::forward<Ts>(ts)...);
        return *this;
    }

    ~Stmt();

 private:
    sqlite3_stmt* m_s{};
};

}  // namespace cent::strg::sqlite
