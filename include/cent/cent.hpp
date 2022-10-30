/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <cent/fs/driver.hpp>
#include <cent/net/driver.hpp>
#include <memory>
#include <string_view>

#include "cent/config.hpp"
#include "cent/log_level.hpp"
#include "cent/result.hpp"

namespace cent {

extern std::reference_wrapper<std::ostream> LOG_STREAM;
extern std::reference_wrapper<std::ostream> OUT_STREAM;

class Cent {
 public:
    Cent(Config conf = {});
    ~Cent();

    Result pull(std::string_view image);
    Result image_list();
    Result create(std::string_view image);

 private:
    class CentImpl;
    std::unique_ptr<CentImpl> m_impl;
};

void init_defaults();
}  // namespace cent
