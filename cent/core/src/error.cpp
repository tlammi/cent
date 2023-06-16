

#include "cent/core/error.hpp"
namespace cent {

Error::Error() {}
Error::Error(std::string_view s) : m_what{s} {}
Error::Error(const char* s) : m_what{s} {}
Error::Error(std::string s) : m_what{std::move(s)} {}

void Error::raise() const { throw Exception(m_what); }

}  // namespace cent
