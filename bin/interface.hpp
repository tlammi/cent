#pragma once

#include "cent.hpp"

class Interface final : public cent::Interface {
 public:
    std::unique_ptr<cent::HttpSession> http_session() override {
        return cent::default_http_session();
    }

 private:
};
