#include <cent/core.hpp>
#include <iostream>

int main() {
    cent::logs_init(cent::LogLevel::Trace);
    auto i = LOG_EXPR(1 + 2);
    return i;
}
