#pragma once

#include <cent/concepts.hpp>
#include <cent/dt/error.hpp>
#include <cent/dt/tags.hpp>
#include <cent/util/assert.hpp>
#include <memory>

// NOLINTBEGIN(cppcoreguidelines-pro-type-union-access)

namespace cent {
template <class V, class E>
class BasicResult;

namespace result_concepts {

template <class T>
struct is_result : std::false_type {};

template <class V, class E>
struct is_result<BasicResult<V, E>> : std::true_type {};

template <class T>
constexpr bool is_result_v = is_result<T>::value;

template <class T, class V, class E>
concept convertible_to_value_only =
    std::convertible_to<T, V> && !(std::convertible_to<T, E>);

template <class T, class V, class E>
concept convertible_to_error_only =
    !std::convertible_to<T, V> && std::convertible_to<T, E>;

template <class T, class V, class E>
concept implicit_value_param =
    !is_result_v<T> && convertible_to_value_only<T, V, E>;

template <class T, class V, class E>
concept implicit_error_param =
    !is_result_v<T> && convertible_to_error_only<T, V, E>;

template <class V, class E>
concept move_ctor_valid =
    std::move_constructible<V> && std::move_constructible<E>;

template <class V, class E>
concept move_assignment_valid =
    concepts::move_assignable<V> && concepts::move_assignable<E>;

template <class V, class E>
concept copy_ctor_valid =
    std::copy_constructible<V> && std::copy_constructible<E>;
template <class V, class E>
concept copy_assignment_valid =
    concepts::copy_assignable<V> && concepts::copy_assignable<E>;

}  // namespace result_concepts

template <class V, class E>
class BasicResult {
 public:
    constexpr BasicResult() : m_has_val(true) {}

    template <result_concepts::implicit_value_param<V, E> T>
    BasicResult(T&& t) : BasicResult(cent::value, std::forward<T>(t)) {}

    template <result_concepts::implicit_error_param<V, E> T>
    BasicResult(T&& t) : BasicResult(cent::error, std::forward<T>(t)) {}

    template <class... Ts>
    constexpr BasicResult(value_t /*val*/, Ts&&... ts) : m_has_val(true) {
        std::construct_at(&m_val, std::forward<Ts>(ts)...);
    }

    template <class... Ts>
    constexpr BasicResult(error_t /*err*/, Ts&&... ts) : m_has_val(false) {
        std::construct_at(&m_err, std::forward<Ts>(ts)...);
    }

    constexpr BasicResult(const BasicResult& other)
        requires result_concepts::copy_ctor_valid<V, E>
        : m_has_val(other.m_has_val) {
        if (m_has_val)
            std::construct_at(&m_val, other.m_val);
        else
            std::construct_at(&m_err, other.m_err);
    }

    constexpr BasicResult& operator=(const BasicResult& other)
        requires result_concepts::copy_assignment_valid<V, E>
    {
        destroy();
        m_has_val = other.m_has_val;
        if (m_has_val)
            std::construct_at(&m_val, other.m_val);
        else
            std::construct_at(&m_err, other.m_err);
        return *this;
    }

    constexpr BasicResult(BasicResult&& other)
        requires result_concepts::move_ctor_valid<V, E>
        : m_has_val(other.m_has_val) {
        if (m_has_val)
            std::construct_at(&m_val, std::move(other.m_val));
        else
            std::construct_at(&m_err, std::move(other.m_err));
    }

    BasicResult& operator=(BasicResult&& other)
        requires result_concepts::move_assignment_valid<V, E>
    {
        destroy();
        m_has_val = other.m_has_val;
        if (m_has_val)
            std::construct_at(&m_val, std::move(other.m_val));
        else
            std::construct_at(&m_err, std::move(other.m_err));
        return *this;
    }

    constexpr ~BasicResult() { destroy(); }

    constexpr bool has_value() const noexcept { return m_has_val; }
    constexpr explicit operator bool() const noexcept { return m_has_val; }

    constexpr V& value() {
        CENT_ASSERT(m_has_val);
        return m_val;
    }

    constexpr const V& value() const {
        CENT_ASSERT(m_has_val);
        return m_val;
    }

    constexpr E& error() {
        CENT_ASSERT(!m_has_val);
        return m_err;
    }

    constexpr const E& error() const {
        CENT_ASSERT(!m_has_val);
        return m_err;
    }

    constexpr V& operator*() { return value(); }

    constexpr const V& operator*() const { return value(); }

    constexpr V* operator->() {
        CENT_ASSERT(m_has_val);
        return &m_val;
    }

    constexpr const V* operator->() const {
        CENT_ASSERT(m_has_val);
        return &m_val;
    }

 private:
    void destroy() {
        if (m_has_val)
            std::destroy_at(&m_val);
        else
            std::destroy_at(&m_err);
    }
    union {
        V m_val;
        E m_err;
    };
    bool m_has_val{true};
};

template <class V>
using Result = BasicResult<V, Error>;

template <class V, class E, class... Ts>
constexpr BasicResult<V, E> make_value(Ts&&... ts) {
    return {cent::value, std::forward<Ts>(ts)...};
}

template <class V, class E, class... Ts>
constexpr BasicResult<V, E> make_error(Ts&&... ts) {
    return {cent::error, std::forward<Ts>(ts)...};
}
}  // namespace cent

// NOLINTEND(cppcoreguidelines-pro-type-union-access)
