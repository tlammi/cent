#pragma once

#include <cent/cli/concepts.hpp>
#include <cent/cli/flag.hpp>
#include <cent/cli/lexer.hpp>
#include <cent/cli/opt.hpp>
#include <cent/cli/pos_arg.hpp>
#include <cent/concepts.hpp>
#include <cent/types.hpp>
#include <cent/types/result.hpp>
#include <print>
#include <span>
#include <string_view>

namespace cent::cli {
namespace parse_detail {

class Parser {
 public:
    virtual ~Parser() = default;
    virtual Result<void> parse(std::span<const char* const> args) const = 0;
};

class Base {
 public:
    constexpr std::string_view name() const noexcept { return m_name; }
    constexpr std::string_view desc() const noexcept { return m_desc; }
    constexpr const std::vector<Flag>& flags() const noexcept {
        return m_flags;
    }
    constexpr const std::vector<Opt>& opts() const noexcept { return m_opts; }

 protected:
    constexpr Base(std::string_view name, std::string_view desc,
                   std::vector<Flag> flags, std::vector<Opt> opts)
        : m_name{name},
          m_desc{desc},
          m_flags{std::move(flags)},
          m_opts{std::move(opts)} {}

    constexpr ~Base() = default;

 private:
    std::string_view m_name;
    std::string_view m_desc;
    std::vector<Flag> m_flags;
    std::vector<Opt> m_opts;
};
}  // namespace parse_detail

class Leaf final : public parse_detail::Base, public parse_detail::Parser {
    friend class LeafBuilder;

 public:
    Result<void> parse(int argc, const char* const* argv) const {
        return parse(
            std::span<const char* const>(argv, static_cast<size_t>(argc)));
    }
    Result<void> parse(std::span<const char* const> spn) const override;

 private:
    constexpr Leaf(std::string_view name, std::string_view desc,
                   std::vector<Flag> flags, std::vector<Opt> opts,
                   std::vector<PosArg> pos_args)
        : Base(name, desc, std::move(flags), std::move(opts)),
          m_pos_args(std::move(pos_args)) {}
    std::vector<PosArg> m_pos_args;
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
        return Leaf{m_name, m_desc, std::move(m_flags), std::move(m_opts),
                    std::move(m_pos_args)};
    }

 private:
    std::string_view m_name;
    std::string_view m_desc;
    std::vector<Opt> m_opts{};
    std::vector<Flag> m_flags{};
    std::vector<PosArg> m_pos_args{};
};

class Branch;

class Branch {
 public:
 private:
    std::vector<Inline<parse_detail::Parser>> m_subcmds{};
};
class BranchBuilder {
 public:
 private:
};

}  // namespace cent::cli
