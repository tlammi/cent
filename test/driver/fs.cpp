#include <gtest/gtest.h>

#include <cent/cent.hpp>

class FsTest : public ::testing::Test {
 public:
    FsTest() : fs{cent::fs::builtin_driver("linux")} {}

    std::shared_ptr<cent::fs::Driver> fs;
};

TEST_F(FsTest, ExistsThis) { ASSERT_TRUE(fs->exists(".")); }
TEST_F(FsTest, MkDir) {
    ASSERT_FALSE(fs->rm("test_fs-dir", true, true));
    ASSERT_FALSE(fs->mkdir("test_fs-dir"));
    ASSERT_TRUE(fs->exists("test_fs-dir"));
}