#pragma once

namespace cent {

class File {
 public:
    constexpr File() = default;
    explicit File(const char* path);

 private:
    int m_fd{};
};
}  // namespace cent
