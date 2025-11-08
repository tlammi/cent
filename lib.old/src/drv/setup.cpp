#include "cent/drv/setup.hpp"

#include "cent/drv/file_system_impl.hpp"
#include "cent/drv/http_session_impl.hpp"
#include "cent/drv/sandbox_impl.hpp"
#include "cent/drv/unpacker_impl.hpp"

namespace cent::drv {
namespace {

struct Init {
    Init() {
        register_file_system("default", fs.get());
        register_http_session("default", default_http_session);
        register_sandbox("default", default_sandbox);
        register_unpacker("default", up.get());
    }
    std::unique_ptr<FileSystem> fs{default_file_system()};
    std::unique_ptr<Unpacker> up{default_unpacker()};
};

const Init init{};
}  // namespace

void setup(const Drivers& drv_conf) {
    set_file_system(drv_conf.file_system);
    set_http_session(drv_conf.http_session);
    set_sandbox(drv_conf.sandbox);
    set_unpacker(drv_conf.unpacker);
}
}  // namespace cent::drv