#pragma once

namespace cent {

enum class ErrorCode {
    Perm,
};

class Error {
 public:
    Error(ErrorCode ec);

 private:
};

}  // namespace cent
