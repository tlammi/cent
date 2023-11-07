#pragma once

#include <optional>

namespace cent::core::cli {

class ArgIter {
 public:
    ArgIter(int argc, char** argv) : m_argc(argc), m_argv(argv) {}

    std::optional<char*> next() noexcept {
        if (m_idx < m_argc) {
            ++m_idx;
            return m_argv[m_idx - 1];
        }
        return std::nullopt;
    }

 private:
    int m_idx{0};
    int m_argc;
    char** m_argv;
};
}  // namespace cent::core::cli
