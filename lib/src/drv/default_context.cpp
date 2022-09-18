/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include "cent/drv/context.hpp"
#include "cent/raise.hpp"

namespace cent::drv {

class DefaultContext final : public Context {
 public:
    stdfs::path storage_path() override {
        auto* home = std::getenv("HOME");
        if (!home) raise("foo");
        stdfs::path path{home};
        return path / ".cent/storage";
    }
    stdfs::path workspace_path() override {
        auto* home = std::getenv("HOME");
        if (!home) raise("No HOME");
        stdfs::path path{home};
        return path / ".cent/workspace";
    }
};
std::unique_ptr<Context> default_context() {
    return std::make_unique<DefaultContext>();
}
}  // namespace cent::drv