#pragma once

#include <string>
#include <vector>

namespace cent::drv {

using StrVect = std::vector<std::string_view>;

class Process {
 public:
    Process();

    Process& args(const StrVect& a);
    Process& env(const StrVect& e);
    Process& out(std::string& o);
    Process& err(std::string& e);
    Process& in(const std::string* i);

    int run();

 private:
    const StrVect* m_args{};
    const StrVect* m_env{};

    std::string* m_out{nullptr};
    std::string* m_err{nullptr};
    const std::string* m_in{nullptr};
};

}  // namespace cent::drv