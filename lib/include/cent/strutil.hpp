#pragma once

#include <string>

#include "cent/concepts.hpp"

namespace cent {

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
requires(concepts::implicit_const_iterable<Container>&&
             concepts::maybe_empty<Container>) std::string
    join(Container&& container, std::string_view token) {
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
}  // namespace cent