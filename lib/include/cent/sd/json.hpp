
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace cent::sd::json {

class Json;

using Null = std::nullptr_t;
using Bool = bool;
using Int = std::int64_t;
using Float = long double;
using Str = std::string;
using StrView = std::string_view;
using Obj = std::map<std::string, Json, std::less<>>;
using Arr = std::vector<Json>;

using Variant = std::variant<Null, Bool, Int, Float, Str, Obj, Arr>;

class Json {
 public:
    constexpr Json() noexcept = default;
    constexpr explicit Json(Null) noexcept : m_var{} {}
    constexpr explicit Json(Bool b) noexcept
        : m_var{std::in_place_type<Bool>, b} {}

    template <std::integral I>
    constexpr explicit Json(I i) noexcept : m_var{std::in_place_type<Int>, i} {}

    template <std::floating_point F>
    constexpr explicit Json(F f) noexcept
        : m_var{std::in_place_type<Float>, f} {}

    constexpr explicit Json(Str s) noexcept
        : m_var{std::in_place_type<Str>, std::move(s)} {}

    constexpr explicit Json(StrView s) : Json(Str{s}) {}
    template <size_t S>
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays)
    constexpr explicit Json(const char (&s)[S]) : Json(StrView{s}) {}
    constexpr explicit Json(const char* s) : Json(StrView{s}) {}

    explicit Json(Obj o) noexcept
        : m_var{std::in_place_type<Obj>, std::move(o)} {}

    explicit Json(Arr a) noexcept
        : m_var{std::in_place_type<Arr>, std::move(a)} {}

    constexpr explicit operator bool() const noexcept { return !is_null(); }

    constexpr bool is_null() const noexcept {
        return std::holds_alternative<Null>(m_var);
    }

    constexpr bool is_bool() const noexcept {
        return std::holds_alternative<Bool>(m_var);
    }

    constexpr bool is_int() const noexcept {
        return std::holds_alternative<Int>(m_var);
    }

    constexpr bool is_float() const noexcept {
        return std::holds_alternative<Float>(m_var);
    }

    constexpr bool is_str() const noexcept {
        return std::holds_alternative<Str>(m_var);
    }

    constexpr bool is_obj() const noexcept {
        return std::holds_alternative<Obj>(m_var);
    }

    constexpr bool is_arr() const noexcept {
        return std::holds_alternative<Arr>(m_var);
    }

    constexpr bool& as_bool() noexcept {
        assert(is_bool());
        return *std::get_if<Bool>(&m_var);
    }
    constexpr const bool& as_bool() const noexcept {
        assert(is_bool());
        return *std::get_if<Bool>(&m_var);
    }

    constexpr Int& as_int() noexcept {
        assert(is_int());
        return *std::get_if<Int>(&m_var);
    }

    constexpr const Int& as_int() const noexcept {
        assert(is_int());
        return *std::get_if<Int>(&m_var);
    }

    constexpr Float& as_float() noexcept {
        assert(is_float());
        return *std::get_if<Float>(&m_var);
    }

    constexpr const Float& as_float() const noexcept {
        assert(is_float());
        return *std::get_if<Float>(&m_var);
    }

    constexpr Str& as_str() noexcept {
        assert(is_str());
        return *std::get_if<Str>(&m_var);
    }

    constexpr const Str& as_str() const noexcept {
        assert(is_str());
        return *std::get_if<Str>(&m_var);
    }

    constexpr Obj& as_obj() noexcept {
        assert(is_obj());
        return *std::get_if<Obj>(&m_var);
    }

    constexpr const Obj& as_obj() const noexcept {
        assert(is_obj());
        return *std::get_if<Obj>(&m_var);
    }

    constexpr Arr& as_arr() noexcept {
        assert(is_arr());
        return *std::get_if<Arr>(&m_var);
    }

    constexpr const Arr& as_arr() const noexcept {
        assert(is_arr());
        return *std::get_if<Arr>(&m_var);
    }

    constexpr Json& operator[](StrView s) {
        if (is_null()) { m_var.emplace<Obj>(); }
        auto& obj = as_obj();
        auto iter = obj.find(s);
        if (iter == obj.end()) {
            auto [i, success] = obj.try_emplace(Str(s));
            assert(success);
            return i->second;
        }
        return iter->second;
    }

 private:
    Variant m_var{};
};

struct ParseError {
    std::string_view msg{};
    size_t line{};
    size_t column{};
};

std::expected<Json, ParseError> parse(std::string_view s);

namespace literals {
constexpr Json operator""_json(const char* c, size_t s) {
    return Json{std::string_view(c, s)};
}
constexpr Json operator""_json(unsigned long long i) { return Json{i}; }
constexpr Json operator""_json(long double f) { return Json{f}; }
}  // namespace literals

}  // namespace cent::sd::json
