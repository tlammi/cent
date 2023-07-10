#include <CLI/CLI.hpp>
#include <CLI/Error.hpp>
#include <cent/reg.hpp>
#include <cstdlib>

#include "cent/core/logs.hpp"
#include "cent/dm/reference.hpp"
#include "cent/reg/registry.hpp"

int do_pull_manifest_list(const std::string& str) {
    cent::dm::Reference ref{str};
    cent::reg::Registry reg{std::string(ref.registry())};
    auto ml = reg.manifest_list(ref.repo(), ref.tag());
    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
    CLI::App app{"Tool for interacting with container registries"};
    cent::LogLevel log_level{cent::LogLevel::Warn};
    app.add_option_function<std::string>(
        "-l,--log-level",
        [&](const std::string& s) { log_level = cent::to_log_level(s); },
        "Log level");
    auto* pull = app.add_subcommand("pull", "Pull from registry");

    bool pull_manifest_list = false;
    pull->add_flag("--manifest-list", pull_manifest_list,
                   "Pull a manifest list");

    std::string pull_ref;
    pull->add_option("ref", pull_ref,
                     "Resource reference. E.g. docker.io/library/alpine:latest")
        ->required();

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        app.exit(e);
        return EXIT_FAILURE;
    }

    cent::logs_init(log_level);
    if (*pull && pull_manifest_list) return do_pull_manifest_list(pull_ref);
    CENT_ERR("Bad usage");
    return EXIT_FAILURE;
}
