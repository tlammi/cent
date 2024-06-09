#include <cent/cli/parser.hpp>

namespace cent::cli {
namespace parser_detail {
namespace {

template <class T>
static auto lookup_short(T& span, char shortf) -> const
    typename std::remove_cvref_t<T>::value_type* {
    for (auto& v : span) {
        if (v.shortf == shortf) return &v;
    }
    return nullptr;
}

template <class T>
static auto lookup_long(T& span, std::string_view longf) -> const
    typename std::remove_cvref_t<T>::value_type* {
    for (auto& v : span) {
        if (v.longf == longf) return &v;
    }
    return nullptr;
}

Result<void> parse_opts_short(auto& lexeme, auto& lexer,
                              const std::vector<Flag>& flags,
                              const std::vector<Opt>& opts) {
    using enum ArgType;
    const auto* flag = lookup_short(flags, lexeme.value.front());
    if (flag) {
        *flag->store = true;
        return {};
    }
    const auto* opt = lookup_short(opts, lexeme.value.front());
    if (opt) {
        lexeme = lexer();
        switch (lexeme.type) {
            case End:
                return error(
                    ErrorCode::Noent,
                    std::format("missing argument for --{}", opt->longf));
            case Short:
            case Long:
            case Value: return opt->value->parse(lexeme.value);
        }
    }
    return error(ErrorCode::Inval,
                 std::format("unknown flag: -{}", lexeme.value.front()));
}

Result<void> parse_opts_long(auto& lexeme, auto& lexer,
                             const std::vector<Flag>& flags,
                             const std::vector<Opt>& opts) {
    using enum ArgType;
    const auto* flag = lookup_long(flags, lexeme.value);
    if (flag) {
        *flag->store = true;
        return {};
    }
    const auto* opt = lookup_long(opts, lexeme.value);
    if (opt) {
        lexeme = lexer();
        switch (lexeme.type) {
            case End:
                return error(
                    ErrorCode::Noent,
                    std::format("missing argument for --{}", opt->longf));
            case Short:
            case Long:
            case Value: return opt->value->parse(lexeme.value);
        }
    }
    return error(ErrorCode::Inval,
                 std::format("unknown flag: --{}", lexeme.value));
}

Result<void> parse_opts(auto& lexeme, auto& lexer,
                        const std::vector<Flag>& flags,
                        const std::vector<Opt>& opts) {
    while (true) {
        using enum ArgType;
        switch (lexeme.type) {
            case End: return {};
            case Short:
                if (auto res = parse_opts_short(lexeme, lexer, flags, opts);
                    !res)
                    return res;
                break;
            case Long:
                if (auto res = parse_opts_long(lexeme, lexer, flags, opts);
                    !res)
                    return res;
                break;
            case Value: return {};
        }
        lexeme = lexer();
    }
    return {};
}

}  // namespace

}  // namespace parser_detail

Result<void> Leaf::parse(std::span<const char* const> spn) const {
    using enum Nargs;
    auto lexer = make_lexer(spn);
    auto arg = lexer();
    auto res = parser_detail::parse_opts(arg, lexer, m_flags, m_opts);
    if (!res) return res;
    auto iter = m_pos_args.begin();
    while (arg) {
        if (iter == m_pos_args.end()) return error(ErrorCode::Toobig);
        auto res = iter->value->parse(arg.value);
        if (!res) return res;
        switch (iter->value->nargs()) {
            case None: panic();
            case One: ++iter; break;
            case Some: break;
            case More: break;
        }
        arg = lexer();
    }
    return {};
}
}  // namespace cent::cli
