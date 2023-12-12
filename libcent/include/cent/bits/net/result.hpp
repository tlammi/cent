#pragma once
#include <cent/dt.hpp>
#include <cstdint>

namespace cent::net {

using Status = std::uint8_t;

enum class ErrorCode {
    Cancelled,

};

struct Error {
    ErrorCode code;
    Error(ErrorCode c) : code(c) {}
};

using Result = cent::BasicResult<Status, Error>;

}  // namespace cent::net
