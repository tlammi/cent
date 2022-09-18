/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include "cent.hpp"

class Drivers final : public cent::drv::Drivers {
 public:
    std::unique_ptr<cent::drv::HttpSession> http_session() override {
        return cent::drv::default_http_session();
    }

    cent::drv::FileSystem* file_system() override { return m_fs.get(); }

    cent::drv::Context* context() override { return m_ctx.get(); }

    std::unique_ptr<cent::drv::Sandbox> sandbox() override {
        return cent::drv::default_sandbox();
    }

    cent::drv::Unpacker* unpacker() override { return m_unpacker.get(); }

 private:
    std::unique_ptr<cent::drv::FileSystem> m_fs{
        cent::drv::default_file_system()};
    std::unique_ptr<cent::drv::Context> m_ctx{cent::drv::default_context()};
    std::unique_ptr<cent::drv::Unpacker> m_unpacker{
        cent::drv::default_unpacker()};
};
