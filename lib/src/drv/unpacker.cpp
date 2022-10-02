#include "cent/drv/container.hpp"
#include "cent/drv/unpacker_impl.hpp"
#include "cent/raise.hpp"

namespace cent::drv {

namespace {
Unpacker* g_unpacker = nullptr;

auto& unpacker_container() { return g_container<Unpacker*>; }

struct Init {
 public:
    Init() {
        unpacker_container()["default"] = unpacker.get();
        g_unpacker = unpacker.get();
    }

 private:
    std::unique_ptr<Unpacker> unpacker = default_unpacker();
};

}  // namespace

void register_unpacker(std::string name, Unpacker* unpacker) {
    unpacker_container()[std::move(name)] = unpacker;
}
std::vector<std::string_view> list_unpackers() {
    return container_names(unpacker_container());
}
void set_unpacker(std::string_view name) {
    auto iter = unpacker_container().find(name);
    if (iter == unpacker_container().end())
        raise("Invalid unpacker driver: ", name);
    g_unpacker = iter->second;
}
Unpacker& unpacker() {
    if (!g_unpacker) raise("Unpacker driver not set");
    return *g_unpacker;
}
}  // namespace cent::drv