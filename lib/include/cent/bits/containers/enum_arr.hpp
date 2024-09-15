/**
 * \file
 * */
#pragma once

#include <array>
#include <cent/concepts.hpp>
#include <cent/util.hpp>
#include <magic_enum.hpp>

namespace cent {
/**
 * \brief Array where indexing is done with enumerations
 * */
template <concepts::enum_type E, class T>
class EnumArr {
 public:
    constexpr size_t size() const noexcept { return m_arr.size(); }

 private:
    std::array<T, magic_enum::enum_count<E>()> m_arr{};
};

}  // namespace cent

