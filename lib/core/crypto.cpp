#include <openssl/ssl.h>

#include <cent/core/crypto.hpp>
#include <format>

namespace cent {

std::string sha256(std::span<const std::byte> data) {
    auto buf = std::array<uint8_t, SHA256_DIGEST_LENGTH>{};
    auto* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
    EVP_DigestUpdate(ctx, data.data(), data.size());
    unsigned int digest_len{};
    EVP_DigestFinal_ex(ctx, buf.data(), &digest_len);
    EVP_MD_CTX_free(ctx);
    std::string out{};
    out.resize(SHA256_DIGEST_LENGTH * 2);
    auto it = out.begin();
    for (auto byte : buf) { it = std::format_to(it, "{:02x}", byte); }
    return out;
}

}  // namespace cent
