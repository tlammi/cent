/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <memory>
#include <vector>

#include "cent/def.hpp"

namespace cent::drv {

/**
 * Class capable of extracting various packages, like .tar.gz
 */
class Unpacker {
 public:
    virtual ~Unpacker() {}

    virtual void unpack(const stdfs::path& src, const stdfs::path& dst) = 0;
};

[[deprected]] std::unique_ptr<Unpacker> default_unpacker();

void register_unpacker(std::string name, Unpacker* unpacker);
std::vector<std::string_view> list_unpackers();

}  // namespace cent::drv