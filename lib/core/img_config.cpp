#include <simdjson.h>

#include <cent/core/img_config.hpp>
#include <cent/util/split.hpp>

namespace cent {
namespace img_cfg_detail {

using namespace std::literals;

struct CfgData {
    std::optional<PlatformView> platform{};
    std::optional<ImgConfig2::Config> cfg{};
};

class Impl {
 public:
    virtual ~Impl() = default;

    virtual PlatformView platform() = 0;
    virtual ImgConfig2::Config config() = 0;
    virtual std::string_view data() const noexcept = 0;
};
namespace {

auto parse_env(auto env) {
    std::map<std::string_view, std::string_view> out{};
    for (auto value : env) {
        auto [k, v] = util::split_first(std::string_view(value), "="sv);
        out[k] = v;
    }
    return out;
}

auto parse_cmd(auto cmd) {
    std::vector<std::string_view> out{};
    for (auto v : cmd) { out.push_back(v); }
    return out;
}

template <class T>
class Real final : public Impl {
 public:
    using value_type = typename T::value_type;

    explicit Real(T v) noexcept : m_v(std::move(v)) {
        auto orig_size = m_v.size();
        auto required_size = m_v.size() + simdjson::SIMDJSON_PADDING;
        m_v.reserve(required_size);
        while (m_v.size() < m_v.capacity()) m_v.push_back(value_type{});
        auto view = std::string_view(reinterpret_cast<const char*>(m_v.data()),
                                     orig_size);
        m_doc = m_parser.iterate(view, required_size);
    }

    PlatformView platform() override {
        if (!m_data.platform)
            m_data.platform = PlatformView{
                .os = m_doc["os"],
                .arch = m_doc["architecture"],
            };
        return *m_data.platform;
    }

    ImgConfig2::Config config() override {
        if (!m_data.cfg)
            m_data.cfg = ImgConfig2::Config{
                .env = parse_env(m_doc["config"]["Env"]),
                .cmd = parse_cmd(m_doc["config"]["Cmd"]),
                .working_dir = m_doc["config"]["WorkingDir"],
            };
        return *m_data.cfg;
    }

    std::string_view data() const noexcept override {
        return {m_v.data(), m_v.size()};
    }

 private:
    T m_v;
    simdjson::ondemand::parser m_parser{};
    simdjson::simdjson_result<simdjson::ondemand::document> m_doc{};
    CfgData m_data{};
};
}  // namespace

}  // namespace img_cfg_detail
ImgConfig2::ImgConfig2(std::string s)
    : m_impl(
          std::make_unique<img_cfg_detail::Real<std::string>>(std::move(s))) {}

std::string_view ImgConfig2::data() const noexcept { return m_impl->data(); }

auto ImgConfig2::config() -> Config { return m_impl->config(); }

PlatformView ImgConfig2::platform() { return m_impl->platform(); }

}  // namespace cent
