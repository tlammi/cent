#pragma once

namespace cent {

class Abstract {
 public:
    constexpr Abstract() noexcept = default;
    Abstract(const Abstract&) = delete;
    Abstract& operator=(const Abstract&) = delete;
    Abstract(Abstract&&) = delete;
    Abstract& operator=(Abstract&&) = delete;

    constexpr virtual ~Abstract() = default;
};
}  // namespace cent
