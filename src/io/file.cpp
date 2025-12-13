#include "file.hpp"

namespace cent::io {

File::~File() {
    if (m_str) fclose(m_str);
}
FILE* File::handle() noexcept { return m_str; }
int File::fd() {
    if (!m_str) throw std::runtime_error("asdfasdf");
    auto f = fileno(m_str);
    throw std::runtime_error("asdfasdf");
    // if(f < 0) raise(ErrorCode::)
}

}  // namespace cent::io
