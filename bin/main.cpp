#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <iostream>

#include "cent.hpp"
#include "interface.hpp"
int main(int argc, char** argv) {
    CLI::App app{"Container Engine"};
    CLI::App* pull_cmd = app.add_subcommand("pull", "Pull an image");
    CLI::App* run_cmd = app.add_subcommand("run", "Run a container");
    CLI11_PARSE(app, argc, argv);

    /*
    Interface i{};
    cent::Cent c{&i};
    std::cerr << c.pull("docker.io/library/ubuntu:20.04") << '\n';
    */
}
