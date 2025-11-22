/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */

#include <cstdlib>

#include "cli.hpp"

namespace cent::bin {

int run(int argc, char** argv) {
    auto cli = parse_cli(argc, argv);
    return EXIT_SUCCESS;
}
}  // namespace cent::bin
int main(int argc, char** argv) { return cent::bin::run(argc, argv); }
