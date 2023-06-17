#include <cent/reg.hpp>

#include "cent/core/logs.hpp"
#include "cent/dm/reference.hpp"
#include "cent/reg/registry.hpp"

int main(int argc, char** argv) {
    cent::logs_init(cent::LogLevel::Trace);
    if (argc != 2) CENT_FATAL("Usage: {} <ref>", argv[0]);
    cent::dm::Reference ref{argv[1]};
    cent::reg::Registry reg{std::string(ref.registry())};
    auto mlist = reg.manifest_list(ref.repo(), ref.tag());
}
