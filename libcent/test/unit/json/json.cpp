#include <gtest/gtest.h>

#include <cent/json/json.hpp>

TEST(Json, Empty) {
    cent::json::Json j;
    ASSERT_TRUE(j.is_null());
}
TEST(Json, Bool) {
    cent::json::Json j{true};
    ASSERT_TRUE(j.is_bool());
}

TEST(Json, Int) {
    cent::json::Json j{42};
    ASSERT_EQ(j.as_int(), 42);
}

TEST(Json, Float) {
    cent::json::Json j{42.0f};
    ASSERT_EQ(j.as_float(), 42.0f);
}

TEST(Json, Str) {
    cent::json::Json j{"hello"};
    ASSERT_EQ(j.as_str(), "hello");
}

TEST(Json, Arr) {
    cent::json::Json j{cent::json::Arr(std::in_place, 1, 2, 3, "foo")};
    ASSERT_EQ(j.as_arr().size(), 4);
    ASSERT_EQ(j.as_arr()[0].as_int(), 1);
    ASSERT_EQ(j.as_arr()[1].as_int(), 2);
    ASSERT_EQ(j.as_arr()[2].as_int(), 3);
    ASSERT_EQ(j.as_arr()[3].as_str(), "foo");
}

TEST(Json, Obj) {
    cent::json::Json j{cent::json::Obj{{"a", 1}, {"b", 2}}};
    ASSERT_EQ(j.as_obj().size(), 2);
    ASSERT_EQ(j.as_obj()["a"].as_int(), 1);
    ASSERT_EQ(j.as_obj()["b"].as_int(), 2);
}
