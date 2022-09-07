#pragma once

#include "cent.hpp"

class Interface final : public cent::Interface {
 public:
    std::unique_ptr<cent::HttpSession> http_session() override {
        return cent::default_http_session();
    }

    cent::FileSystemApi* file_system() override { return m_fs.get(); }

 private:
    std::unique_ptr<cent::FileSystemApi> m_fs{cent::default_file_system_api()};
};
