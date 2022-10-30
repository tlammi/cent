#include "cent/fs/lock_file.hpp"

#include <gtest/gtest.h>

#include <cent/cent.hpp>

TEST(LockFile, Ctor) {
    cent::init_defaults();
    cent::fs::LockFile{};
    cent::fs::LockFile f{"/tmp/tmplock"};
}