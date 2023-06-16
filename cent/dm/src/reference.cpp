
#include <cent/dm.hpp>

namespace cent::dm {
namespace ref_detail {

size_t tag_sep(std::string_view s) {
    size_t idx = s.find(':');
    if (idx == std::string_view::npos) return idx;
    auto substr = s.substr(idx + 1);
    size_t slash = substr.find('/');
    // check if colon was a port separator or a tag separator
    if (slash == std::string_view::npos) return idx;
    return tag_sep(substr);
}

size_t digest_sep(std::string_view s) { return s.find('@'); }

size_t ref_sep(std::string_view s) {
    auto idx = digest_sep(s);
    if (idx != std::string_view::npos) return idx;
    return tag_sep(s);
}
}  // namespace ref_detail
}  // namespace cent::dm
