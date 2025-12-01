#include <simdjson.h>

#include <cent/core/manifest.hpp>

namespace cent {
namespace {

struct ManifestData {
    std::string cfg_digest{};
    std::vector<std::string> layer_digests{};
    std::vector<std::pair<std::string, std::string>> annotations{};
};
}  // namespace
namespace manifest_detail {
class Impl {
 public:
    virtual ~Impl() = default;

    virtual std::string_view cfg_digest() = 0;
    virtual std::span<const std::string> layer_digests() = 0;
    virtual std::span<const std::pair<std::string, std::string>>
    annotations() = 0;

    virtual std::string_view data() = 0;

 private:
};

template <class T>
class Real final : public Impl {
 public:
    using value_type = typename T::value_type;

    explicit Real(T v) : m_v(std::move(v)) {
        auto orig_size = m_v.size();
        auto required_size = m_v.size() + simdjson::SIMDJSON_PADDING;
        m_v.reserve(required_size);
        while (m_v.size() < m_v.capacity()) m_v.push_back(value_type{});
        auto view = std::string_view(reinterpret_cast<const char*>(m_v.data()),
                                     orig_size);
        m_doc = m_parser.iterate(view, required_size);
    }

    std::string_view cfg_digest() override {
        if (m_data.cfg_digest.empty())
            m_data.cfg_digest = std::string(m_doc["config"]["digest"]);
        return m_data.cfg_digest;
    }

    std::span<const std::string> layer_digests() override {
        if (m_data.layer_digests.empty()) {
            for (simdjson::ondemand::value v : m_doc["layers"]) {
                m_data.layer_digests.push_back(std::string(v["digest"]));
            }
        }
        return m_data.layer_digests;
    }

    std::span<const std::pair<std::string, std::string>> annotations()
        override {
        return m_data.annotations;
    }

    std::string_view data() override {
        return {reinterpret_cast<const char*>(m_v.data()), m_v.size()};
    }

 private:
    T m_v;
    simdjson::ondemand::parser m_parser{};
    simdjson::simdjson_result<simdjson::ondemand::document> m_doc{};
    ManifestData m_data{};
};

}  // namespace manifest_detail

Manifest2::Manifest2(std::string v)
    : m_impl(
          std::make_unique<manifest_detail::Real<std::string>>(std::move(v))) {}

Manifest2::Manifest2(std::vector<std::byte> v)
    : m_impl(std::make_unique<manifest_detail::Real<std::vector<std::byte>>>(
          std::move(v))) {}

Manifest2::~Manifest2() = default;

std::string_view Manifest2::config_digest() { return m_impl->cfg_digest(); }

std::vector<std::string_view> Manifest2::layer_digests() {
    return m_impl->layer_digests() |
           std::ranges::to<std::vector<std::string_view>>();
}

std::vector<std::pair<std::string_view, std::string_view>>
Manifest2::annotations() {
    return m_impl->annotations() |
           std::ranges::to<
               std::vector<std::pair<std::string_view, std::string_view>>>();
}

std::string_view Manifest2::data() noexcept { return m_impl->data(); }

}  // namespace cent
