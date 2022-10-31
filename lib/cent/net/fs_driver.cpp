#include "fs_driver.hpp"

namespace cent::net {

class FsHttpSession final : public HttpSession {
 public:
    void capture_header_field(std::string_view field) override {}

    std::string& header_field(std::string_view field) override {
        return m_mock;
    }

    const std::string& header_field(std::string_view field) const override {
        return m_mock;
    }

    void set_header_field(std::string_view field,
                          std::string_view value) override {}

    std::string_view get_body() override { return {}; }

    int get(std::string_view url) override { return 404; }

 private:
    std::string m_mock{""};
};
/**
 * File system backed HTTP driver
 *
 * This is a driver implementation which handles all requests using local fs.
 * Used for testing.
 */
class FsDriver final : public Driver {
 public:
    std::unique_ptr<HttpSession> http_session() override {
        return std::make_unique<FsHttpSession>();
    }
    std::string_view name() const noexcept override { return "fs"; }
};

std::shared_ptr<Driver> make_fs_driver() {
    return std::make_shared<FsDriver>();
}
}  // namespace cent::net