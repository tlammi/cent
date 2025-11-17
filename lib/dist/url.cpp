#include <cent/dist/url.hpp>

namespace cent::dist {

Url manifest_url(NameView nm) {
    auto reg = nm.registry();
    auto repo = nm.repository();
    auto ref = nm.suffix();
    return Url(std::format("https://{}/v2/{}/manifests/{}", reg, repo, ref));
}

}  // namespace cent::dist
