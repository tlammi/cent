#pragma once

#include <cent/cli/concepts.hpp>
#include <cent/cli/flag.hpp>
#include <cent/cli/lexer.hpp>
#include <cent/cli/opt.hpp>
#include <cent/cli/pos_arg.hpp>
#include <cent/concepts.hpp>
#include <cent/types/result.hpp>
#include <print>
#include <span>
#include <string_view>

namespace cent::cli {
namespace parse_detail {
class Base {
 public:
 protected:
    constexpr Base() = default;

 private:
};
}  // namespace parse_detail

class Leaf {
    friend class LeafBuilder;

 public:
    Result<void> parse(int argc, const char* const* argv) const {
        return parse(
            std::span<const char* const>(argv, static_cast<size_t>(argc)));
    }
    Result<void> parse(std::span<const char* const> spn) const;

 private:
    constexpr Leaf(std::string_view name, std::string_view desc,
                   std::vector<PosArg> pos_args, std::vector<Opt> opts,
                   std::vector<Flag> flags)
        : m_name{name},
          m_desc{desc},
          m_pos_args(std::move(pos_args)),
          m_opts(std::move(opts)),
          m_flags(std::move(flags)) {}

    std::string_view m_name;
    std::string_view m_desc;
    std::vector<PosArg> m_pos_args;
    std::vector<Opt> m_opts;
    std::vector<Flag> m_flags;
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
        m_flags.emplace_back(shortf, longf, store, help);
        return *this;
    }

    constexpr LeafBuilder& flag(std::string_view longf, bool* store,
                                std::string_view help) {
        m_flags.emplace_back('\0', longf, store, help);
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
    std::vector<Flag> m_flags{};
    std::vector<PosArg> m_pos_args{};
};

class Branch;
using Cmd = std::variant<Leaf, Branch>;

class Branch {
 public:
 private:
    std::vector<Cmd> m_subcmds{};
};
class BranchBuilder {};

}  // namespace cent::cli
