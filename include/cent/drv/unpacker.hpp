/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <memory>
#include <vector>

#include "cent/def.hpp"
#include "cent/drv/driver.hpp"

namespace cent::drv {

/**
 * Class capable of extracting various packages, like .tar.gz
 */
class Unpacker : public Driver {
 public:
    virtual void unpack(const stdfs::path& src, const stdfs::path& dst) = 0;
};

void register_unpacker(std::string name, Unpacker* unpacker);
std::vector<std::string_view> list_unpackers();

}  // namespace cent::drv