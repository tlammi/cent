#include <openssl/ssl.h>

#include <cent/core/crypto.hpp>
#include <format>

namespace cent {

using namespace std::literals;
std::string sha256(std::span<const std::byte> data) {
    auto buf = std::array<uint8_t, SHA256_DIGEST_LENGTH>{};
    auto* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
    EVP_DigestUpdate(ctx, data.data(), data.size());
    unsigned int digest_len{};
    EVP_DigestFinal_ex(ctx, buf.data(), &digest_len);
    EVP_MD_CTX_free(ctx);
    static constexpr auto prefix = "sha256:"sv;
    std::string out{prefix};
    out.resize(out.size() + SHA256_DIGEST_LENGTH * 2);
    auto it = out.begin() + prefix.size();
    for (auto byte : buf) { it = std::format_to(it, "{:02x}", byte); }
    return out;
}

}  // namespace cent
