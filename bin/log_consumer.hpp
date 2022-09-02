#pragma once

#include <iostream>

#include "cent.hpp"

class LogConsumer final : public cent::LogConsumer {
 public:
    std::ostream& debug() { return std::cerr; }
    std::ostream& info() { return std::cerr; }
    std::ostream& note() { return std::cerr; }
    std::ostream& warn() { return std::cerr; }
    std::ostream& err() { return std::cerr; }
    std::ostream& fatal() { return std::cerr; }

 private:
};