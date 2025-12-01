#pragma once

#include <cent/platform.hpp>
#include <map>
#include <memory>
#include <vector>

namespace cent {
struct ImgConfig {
    std::string digest{};
    Platform platform{};
    struct {
        std::map<std::string, std::string> env{};
        std::vector<std::string> cmd{};
        std::string working_dir{};
    } config;
};

namespace img_cfg_detail {
class Impl;
}

class ImgConfig2 {
 public:
    struct Config {
        std::map<std::string_view, std::string_view> env;
        std::vector<std::string_view> cmd;
        std::string_view working_dir;
    };

    explicit ImgConfig2(std::string s);
    ImgConfig2(const ImgConfig2&) = delete;
    ImgConfig2& operator=(const ImgConfig2&) = delete;

    ImgConfig2(ImgConfig2&&) = default;
    ImgConfig2& operator=(ImgConfig2&&) = default;

    ~ImgConfig2();

    Config config();
    PlatformView platform();

    std::string_view data() const noexcept;

 private:
    std::unique_ptr<img_cfg_detail::Impl> m_impl;
};

}  // namespace cent
