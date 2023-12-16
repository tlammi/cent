#pragma once

#include <memory>

namespace cent::reg {

class Client {
 public:
    virtual ~Client() = default;

    std::string manifest_list(std::string url);

 private:
};

std::unique_ptr<Client> client();
}  // namespace cent::reg
