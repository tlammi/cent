#pragma once

#include <cent/bits/reg/reference.hpp>
#include <memory>

namespace cent::reg {

class Client {
 public:
    virtual ~Client() = default;

    std::string manifest_list(const std::string& s) {
        return manifest_list(Reference{s});
    }

    virtual std::string manifest_list(const Reference& ref) = 0;

 private:
};

std::unique_ptr<Client> client();
}  // namespace cent::reg
