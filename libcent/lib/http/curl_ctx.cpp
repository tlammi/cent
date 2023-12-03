
#include <array>
#include <cent/http/curl_ctx.hpp>
#include <cent/util.hpp>
#include <shared_mutex>

namespace cent::http {
struct Mutex {
    std::shared_mutex mut;
    bool shared;
};
using MutexArr = std::array<Mutex, CURL_LOCK_DATA_LAST>;
namespace {
auto mk_muts() { return Anon(std::in_place_type<MutexArr>); }
}  // namespace

CurlCtx::CurlCtx() : m_muts(mk_muts()), m_c(curl_share_init()) {
    CENT_CHECK(m_c);
    curl_share_setopt(m_c, CURLSHOPT_USERDATA, m_muts.data());
    curl_share_setopt(
        m_c, CURLSHOPT_LOCKFUNC,
        +[](CURL* hndl, curl_lock_data data, curl_lock_access access,
            void* userdata) {
            (void)hndl;
            auto* arr = static_cast<MutexArr*>(userdata);
            switch (access) {
                case CURL_LOCK_ACCESS_SINGLE:
                    arr->at(data).mut.lock();
                    arr->at(data).shared = false;
                    break;
                case CURL_LOCK_ACCESS_SHARED:
                    arr->at(data).mut.lock_shared();
                    arr->at(data).shared = true;
                    break;
                default: abort("Unsupported lock access: ", access);
            }
        });
    curl_share_setopt(
        m_c, CURLSHOPT_UNLOCKFUNC,
        +[](CURL* hndl, curl_lock_data data, void* userdata) {
            (void)hndl;
            auto* arr = static_cast<MutexArr*>(userdata);
            if (arr->at(data).shared)
                arr->at(data).mut.unlock_shared();
            else
                arr->at(data).mut.unlock();
        });
}

CurlCtx::CurlCtx(CurlCtx&& other) noexcept { std::swap(m_c, other.m_c); }

CurlCtx& CurlCtx::operator=(CurlCtx&& other) noexcept {
    if (this != &other) {
        auto tmp = CurlCtx(std::move(other));
        std::swap(m_c, tmp.m_c);
    }
    return *this;
}

CurlCtx::~CurlCtx() { curl_share_cleanup(m_c); }

}  // namespace cent::http
