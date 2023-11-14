#include <cent/json/lex.hpp>
#include <cstdint>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    cent::json::lex_all(
        std::string_view(reinterpret_cast<const char*>(data), size));  // NOLINT
    return 0;
}
