/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "cent/concepts.hpp"

namespace cent::util {
namespace detail {

template <class S, class... Rest>
void do_concat(std::stringstream& ss, S&& s, Rest&&... rest) {
    ss << std::forward<S>(s);
    if constexpr (sizeof...(Rest)) {
        do_concat(ss, std::forward<Rest>(rest)...);
    }
}

template <class Token, class Str, class... Strs>
void do_join(std::stringstream& ss, Token&& token, Str&& str, Strs&&... strs) {
    ss << std::forward<Str>(str);
    if constexpr (sizeof...(Strs)) {
        ss << token;
        do_join(ss, std::forward<Token>(token), std::forward<Strs>(strs)...);
    }
}

}  // namespace detail
/**
 * Join strings in the given container
 *
 * Joins the strings concatenating them with the given token.
 *
 * \param container Container to iterate and join
 * \param token Token to insert between items from \a container
 * \return String with all the elements concatenated
 */
template <class Container>
    requires(concepts::implicit_const_iterable<Container> &&
             concepts::maybe_empty<Container>)
std::string join(Container&& container, std::string_view token) {
    if (container.empty()) return "";
    auto begin = container.begin();
    auto end = container.end();
    std::string out{*begin};
    ++begin;
    while (begin != end) {
        out += token;
        out += *begin;
        ++begin;
    }
    return out;
}

std::pair<std::string_view, std::string_view> split_left(
    std::string_view str, std::string_view token);

std::vector<std::string_view> split(std::string_view str,
                                    std::string_view token);

template <class Token, class... StrLike>
std::string join_inline(Token&& token, StrLike&&... strs) {
    if constexpr (sizeof...(StrLike) == 0)
        return "";
    else {
        std::stringstream ss{};
        detail::do_join(ss, std::forward<Token>(token),
                        std::forward<StrLike>(strs)...);
        return ss.str();
    }
}

template <class... StrLike>
std::string concat_inline(StrLike&&... strs) {
    if constexpr (sizeof...(StrLike) == 0) {
        return "";
    } else {
        std::stringstream ss{};
        detail::do_concat(ss, std::forward<StrLike>(strs)...);
        return ss.str();
    }
}
}  // namespace cent::util