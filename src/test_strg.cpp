#include "module.hpp"

namespace component = cent::component;

struct Ctx final : public cent::ModuleCtx {
    std::vector<const component::Meta<component::Storage>*> metas{};

    void add_component(
        const component::Meta<component::Storage>& meta) override {
        metas.push_back(&meta);
    }
};

struct StrgTuple {
    std::string_view name;
    component::Storage* storage;
};

std::vector<StrgTuple> get_storages() {
    static std::vector<std::unique_ptr<component::Storage>> storages{};
    static std::vector<StrgTuple> out{};
    if (storages.empty()) {
        Ctx c{};
        cent::init_modules(c);
        assert(!c.metas.empty());
        for (const auto& m : c.metas) {
            storages.push_back(m->create());
            out.push_back(
                {.name = m->name(), .storage = storages.back().get()});
        }
        assert(!out.empty());
    }
    return out;
}

class StrgTest : public ::testing::TestWithParam<StrgTuple> {
 protected:
    component::Storage* storage;

    void SetUp() override {
        storage = GetParam().storage;
        ASSERT_TRUE(storage);
    }
};

TEST_P(StrgTest, NoManifests) {
    auto m = storage->manifest_ls();
    ASSERT_TRUE(m.empty());
}

INSTANTIATE_TEST_SUITE_P(AllStorages, StrgTest,
                         testing::ValuesIn(get_storages()),
                         [](const auto& info) {
                             return std::string(info.param.name);
                         });
