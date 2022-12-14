/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <iostream>

#include "cent.hpp"

int main(int argc, char** argv) {
    CLI::App app{"Container Engine"};
    cent::Cent c{};
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

    CLI::App* create_cmd = app.add_subcommand("create", "Create a container");
    create_cmd->add_option("image", image);
    create_cmd->final_callback([&]() { c.create(image); });

    // cent::LOG_LEVEL = cent::LogLevel::Trace;
    cent::LOG_LEVEL = cent::LogLevel::Debug;
    CLI11_PARSE(app, argc, argv);

    /*
    Interface i{};
    cent::Cent c{&i};
    std::cerr << c.pull("docker.io/library/ubuntu:20.04") << '\n';
    */
}
