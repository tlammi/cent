
#include <fmt/format.h>

#include <cent/bits/reg/client.hpp>
#include <cent/net.hpp>

namespace cent::reg {

class ClientImpl final : public Client {
 public:
    using NetClient = net::Client<net::Session>;
    std::string manifest_list(const Reference& ref) override {
        (void)ref;
        std::string url =
            "https://registry-1.docker.io/v2/library/ubuntu/manifests/22:04";
        m_client.on_header([&](std::string_view h, std::string_view v) -> bool {
            std::cerr << "header: " << h << ": " << v << '\n';
            return true;
        });
        m_client.on_write([&](std::string_view buf) -> bool {
            std::cerr << "body: " << buf << '\n';
            return true;
        });
        m_client.url(url);
        m_client.get();
        return "";
    }

 private:
    NetClient m_client{};
};

std::unique_ptr<Client> client() { return std::make_unique<ClientImpl>(); }

}  // namespace cent::reg
