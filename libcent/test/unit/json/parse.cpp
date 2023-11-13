#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cent/json/parse.hpp>

TEST(Value, Empty) {
    auto res = cent::json::parse("");
    ASSERT_FALSE(res);
}

TEST(Value, Null) {
    auto res = cent::json::parse("null");
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->is_null());
}

TEST(Value, True) {
    auto res = cent::json::parse("true");
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->is_bool());
    ASSERT_TRUE(res->as_bool());
}

TEST(Value, False) {
    auto res = cent::json::parse("false");
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->is_bool());
    ASSERT_FALSE(res->as_bool());
}

TEST(Value, Int) {
    auto res = cent::json::parse("42");
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->is_int());
    ASSERT_EQ(res->as_int(), 42);
}

TEST(Value, Float) {
    auto res = cent::json::parse("42.0");
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->is_float());
    ASSERT_EQ(res->as_float(), 42.0f);
}

TEST(Value, Str) {
    auto res = cent::json::parse(R"("hello")");
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->is_str());
    ASSERT_EQ(res->as_str(), "hello");
}

TEST(Value, EmptyStr) {
    auto res = cent::json::parse(R"("")");
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->is_str());
    ASSERT_EQ(res->as_str(), "");
}

TEST(Arr, Empty) {
    auto res = cent::json::parse("[]");
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->is_arr());
    ASSERT_TRUE(res->as_arr().empty());
}

TEST(Arr, Int) {
    auto res = cent::json::parse("[42]");
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->is_arr());
    ASSERT_EQ(res->as_arr().size(), 1);
    ASSERT_EQ(res->as_arr()[0].as_int(), 42);
}

TEST(Arr, NonTerminated) {
    auto res = cent::json::parse("[42");
    ASSERT_FALSE(res);
}

TEST(Arr, TrailingComma) {
    auto res = cent::json::parse("[42,]");
    ASSERT_FALSE(res);
}

TEST(Obj, Empty) {
    auto res = cent::json::parse("{}");
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->is_obj());
    ASSERT_TRUE(res->as_obj().empty());
}

TEST(Obj, Int) {
    auto res = cent::json::parse(R"({"a": 42})");
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->is_obj());
    ASSERT_EQ(res->as_obj().size(), 1);
    ASSERT_EQ(res->as_obj()["a"].as_int(), 42);
}

TEST(Obj, NonTerminated) {
    auto res = cent::json::parse(R"({"a": 42)");
    ASSERT_FALSE(res);
}

TEST(Tree, ArrObj) {
    auto res = cent::json::parse(R"([{"a": 42}])");
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->is_arr());
    ASSERT_EQ(res->as_arr().size(), 1);
    ASSERT_TRUE(res->as_arr()[0].is_obj());
    ASSERT_EQ(res->as_arr()[0].as_obj().size(), 1);
    ASSERT_EQ(res->as_arr()[0].as_obj()["a"].as_int(), 42);
}

TEST(Tree, ObjArr) {
    auto res = cent::json::parse(R"({"a": [42]})");
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->is_obj());
    ASSERT_EQ(res->as_obj().size(), 1);
    ASSERT_TRUE(res->as_obj()["a"].is_arr());
    ASSERT_EQ(res->as_obj()["a"].as_arr().size(), 1);
    ASSERT_EQ(res->as_obj()["a"].as_arr()[0].as_int(), 42);
}

