#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <iostream>

#include "cent.hpp"
#include "interface.hpp"
int main(int argc, char** argv) {
    CLI::App app{"Container Engine"};
    Interface i{};
    cent::Cent c{&i};
    app.require_subcommand();
    CLI::App* pull_cmd = app.add_subcommand("pull", "Pull an image");
    std::string image;
    pull_cmd->add_option("image", image);
    pull_cmd->final_callback([&]() { std::cerr << c.pull(image) << '\n'; });
    CLI::App* run_cmd = app.add_subcommand("run", "Run a container");
    run_cmd->final_callback([]() { std::cerr << "you wanted to run\n"; });
    CLI::App* image_cmd = app.add_subcommand("image", "Work with images");
    auto ls_callback = [&]() { c.image_list(); };
    CLI::App* image_list_cmd = image_cmd->add_subcommand("list", "List images");
    image_list_cmd->final_callback(ls_callback);
    CLI::App* image_ls_cmd = image_cmd->add_subcommand("ls", "List images");
    image_ls_cmd->final_callback(ls_callback);

    cent::LOG_LEVEL = cent::LogLevel::Trace;
    CLI11_PARSE(app, argc, argv);

    /*
    Interface i{};
    cent::Cent c{&i};
    std::cerr << c.pull("docker.io/library/ubuntu:20.04") << '\n';
    */
}
