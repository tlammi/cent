#include <cent/json/parse.hpp>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    cent::json::parse(
        std::string_view(reinterpret_cast<const char*>(data), size));
    return 0;
}
