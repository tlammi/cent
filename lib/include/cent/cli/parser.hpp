#pragma once

#include <cent/cli/concepts.hpp>
#include <cent/cli/lexer.hpp>
#include <cent/cli/opt.hpp>
#include <cent/cli/pos_arg.hpp>
#include <cent/concepts.hpp>
#include <cent/types/result.hpp>
#include <print>
#include <span>
#include <string_view>

namespace cent::cli {

class Leaf {
    friend class LeafBuilder;

 public:
    Result<void> parse(int argc, const char* const* argv) const {
        return parse(
            std::span<const char* const>(argv, static_cast<size_t>(argc)));
    }
    Result<void> parse(std::span<const char* const> spn) const {
        using enum Nargs;
        auto lexer = make_lexer(spn);
        auto arg = lexer();
        while (true) {
            using enum ArgType;
            switch (arg.type) {
                case End: goto opts_processed;
                case Short: {
                    std::println("got short");
                    std::println("{}", m_opts.size());
                    const auto* flag = lookup_short(m_flags, arg.value.front());
                    if (flag) {
                        flag->value->parse("1");
                        break;
                    }
                    const auto* opt = lookup_short(m_opts, arg.value.front());
                    if (opt) {
                        arg = lexer();
                        std::println("arg: {}", arg.value);
                        switch (arg.type) {
                            case End:
                                return error(
                                    ErrorCode::Noent,
                                    std::format("missing argument for --{}",
                                                opt->longf));
                            case Short:
                            case Long:
                            case Value: {
                                std::println("parsing flag");
                                auto res = opt->value->parse(arg.value);
                                if (!res) return res;
                            }
                        }
                    }
                } break;
                case Long: {
                    std::println("got long");
                    std::println("{}", m_opts.size());
                    const auto* flag = lookup_long(m_flags, arg.value);
                    if (flag) {
                        (void)flag->value->parse("1");
                        break;
                    }
                    const auto* opt = lookup_long(m_opts, arg.value);
                    if (opt) {
                        arg = lexer();
                        std::println("arg: {}", arg.value);
                        switch (arg.type) {
                            case End:
                                return error(
                                    ErrorCode::Noent,
                                    std::format("missing argument for --{}",
                                                opt->longf));
                            case Short:
                            case Long:
                            case Value: {
                                std::println("parsing flag");
                                auto res = opt->value->parse(arg.value);
                                if (!res) return res;
                            }
                        }
                        break;
                    }
                } break;

                case Value: goto opts_processed;
            }
            arg = lexer();
        }
    opts_processed:
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

 private:
    constexpr Leaf(std::string_view name, std::string_view desc,
                   std::vector<PosArg> pos_args, std::vector<Opt> opts,
                   std::vector<Opt> flags)
        : m_name{name},
          m_desc{desc},
          m_pos_args(std::move(pos_args)),
          m_opts(std::move(opts)),
          m_flags(std::move(flags)) {}

    static const Opt* lookup_short(std::span<const Opt> span, char shortf) {
        for (auto& v : span) {
            if (v.shortf == shortf) return &v;
        }
        return nullptr;
    }

    static const Opt* lookup_long(std::span<const Opt> span,
                                  std::string_view longf) {
        for (auto& v : span) {
            if (v.longf == longf) return &v;
        }
        return nullptr;
    }

    std::string_view m_name;
    std::string_view m_desc;
    std::vector<PosArg> m_pos_args;
    std::vector<Opt> m_opts;
    std::vector<Opt> m_flags;
};

class LeafBuilder {
 public:
    constexpr LeafBuilder& name(std::string_view name) noexcept {
        m_name = name;
        return *this;
    }

    constexpr LeafBuilder& description(std::string_view desc) noexcept {
        m_desc = desc;
        return *this;
    }

    template <class T>
    constexpr LeafBuilder& pos_arg(std::string_view name, T* store,
                                   std::string_view help) {
        m_pos_args.emplace_back(name, std::make_unique<Value<T>>(store), help);
        return *this;
    }

    template <class T>
    constexpr LeafBuilder& opt(char shortf, std::string_view longf, T* store,
                               std::string_view help) {
        m_opts.emplace_back(shortf, longf, std::make_unique<Value<T>>(store),
                            help);
        return *this;
    }

    template <class T>
    constexpr LeafBuilder& opt(std::string_view longf, T* store,
                               std::string_view help) {
        m_opts.emplace_back('\0', longf, std::make_unique<Value<T>>(store),
                            help);
        return *this;
    }

    constexpr LeafBuilder& flag(char shortf, std::string_view longf,
                                bool* store, std::string_view help) {
        m_flags.emplace_back(shortf, longf,
                             std::make_unique<Value<bool>>(store), help);
        return *this;
    }

    constexpr LeafBuilder& flag(std::string_view longf, bool* store,
                                std::string_view help) {
        m_flags.emplace_back('\0', longf, std::make_unique<Value<bool>>(store),
                             help);
        return *this;
    }

    constexpr Leaf commit() noexcept {
        return Leaf{m_name, m_desc, std::move(m_pos_args), std::move(m_opts),
                    std::move(m_flags)};
    }

 private:
    std::string_view m_name;
    std::string_view m_desc;
    std::vector<Opt> m_opts{};
    std::vector<Opt> m_flags{};
    std::vector<PosArg> m_pos_args{};
};
class Branch {};
class BranchBuilder {};

}  // namespace cent::cli
