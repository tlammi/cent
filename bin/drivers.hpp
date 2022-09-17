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

 private:
    std::unique_ptr<cent::drv::FileSystem> m_fs{
        cent::drv::default_file_system()};
    std::unique_ptr<cent::drv::Context> m_ctx{cent::drv::default_context()};
};
