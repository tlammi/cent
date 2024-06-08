#pragma once

#include <cent/cli/concepts.hpp>
#include <cent/cli/pos_arg.hpp>
#include <cent/concepts.hpp>
#include <cent/types/result.hpp>
#include <span>
#include <string_view>

namespace cent::cli {

class Leaf {
    friend class LeafBuilder;

 public:
    Result<void> parse(int argc, const char* const* argv) {
        return parse(
            std::span<const char* const>(argv, static_cast<size_t>(argc)));
    }
    Result<void> parse(std::span<const char* const> spn) {
        auto iter = m_pos_args.begin();
        using enum Nargs;
        for (const auto* arg : spn) {
            if (iter == m_pos_args.end()) { return error(ErrorCode::Toobig); }
            auto res = iter->value->parse(arg);
            if (!res) return res;
            switch (iter->value->nargs()) {
                case None:
                case One: ++iter; break;
                case Some: break;
                case More: break;
            }
        }
        return {};
    }

 private:
    constexpr Leaf(std::string_view name, std::string_view desc,
                   std::vector<PosArg> pos_args)
        : m_name{name}, m_desc{desc}, m_pos_args{std::move(pos_args)} {}

    std::string_view m_name;
    std::string_view m_desc;
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

    template <class I>
    constexpr LeafBuilder& pos_arg(std::string_view name, I* store,
                                   std::string_view help) {
        m_pos_args.emplace_back(name, std::make_unique<Value<I>>(store), help);
        return *this;
    }
    //
    // template <concepts::string S>
    // constexpr LeafBuilder& pos_arg(std::string_view name, S* store,
    //                                std::string_view help) {
    //     return *this;
    // }

    // template <concepts::vector V>
    // constexpr LeafBuilder& pos_arg(std::string_view name, V* store,
    //                                std::string_view help) {
    //     m_pos_args.emplace_back(name, std::make_unique<Value<V>>(store), help);
    //     return *this;
    // }

    constexpr Leaf commit() noexcept {
        return Leaf{m_name, m_desc, std::move(m_pos_args)};
    }

 private:
    std::string_view m_name;
    std::string_view m_desc;
    std::vector<PosArg> m_pos_args;
};
class Branch {};
class BranchBuilder {};

}  // namespace cent::cli
