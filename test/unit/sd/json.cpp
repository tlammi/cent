#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cent/sd/json.hpp>

namespace json = cent::sd::json;
using namespace json::literals;

TEST(Ctor, Default) {
    json::Json j{};
    ASSERT_TRUE(j.is_null());
    ASSERT_FALSE(j);
}

TEST(Ctor, Null) {
    json::Json j{nullptr};
    ASSERT_TRUE(j.is_null());
    ASSERT_FALSE(j);
}

TEST(Ctor, Bool) {
    json::Json j{false};
    ASSERT_FALSE(j.is_null());
    ASSERT_TRUE(j.is_bool());
    ASSERT_TRUE(j);
    ASSERT_FALSE(j.as_bool());

    j.as_bool() = true;
    ASSERT_TRUE(j.as_bool());
}

TEST(Ctor, Int) {
    json::Json j{0};
    ASSERT_TRUE(j.is_int());
    ASSERT_EQ(j.as_int(), 0);
}

TEST(Ctor, Float) {
    json::Json j{0.0};
    ASSERT_TRUE(j.is_float());
    ASSERT_EQ(j.as_float(), 0);
}

TEST(Ctor, Str) {
    json::Json j{"hello"};
    ASSERT_TRUE(j.is_str());
    ASSERT_EQ(j.as_str(), "hello");
}

TEST(Ctor, Obj) {
    json::Json j{json::Obj{}};
    ASSERT_TRUE(j.is_obj());
    ASSERT_TRUE(j.as_obj().empty());
}

TEST(Ctor, Arr) {
    json::Json j{json::Arr{}};
    ASSERT_TRUE(j.is_arr());
    ASSERT_TRUE(j.as_arr().empty());
}

TEST(Access, Obj) {
    auto j = json::Json();
    j["foo"]["bar"] = "baz"_json;
    j["a"] = json::Json(1);
    ASSERT_EQ(j["a"].as_int(), 1);
    ASSERT_EQ(j["foo"]["bar"].as_str(), "baz");
}

TEST(Parse, Empty) {
    auto j = json::parse("");
    ASSERT_FALSE(j);
}

TEST(Parse, Null) {
    auto j = json::parse("null");
    ASSERT_TRUE(j);
    ASSERT_TRUE(j->is_null());
}

TEST(Parse, Bool) {
    auto j = json::parse("true");
    ASSERT_TRUE(j);
    ASSERT_TRUE(j->as_bool());
    j = json::parse("false");
    ASSERT_TRUE(j);
    ASSERT_FALSE(j->as_bool());
}

TEST(Parse, IntZero) {
    auto j = json::parse("0");
    ASSERT_TRUE(j);
    ASSERT_TRUE(j->is_int());
    ASSERT_EQ(j->as_int(), 0);
}

TEST(Parse, Int) {
    auto j = json::parse("123");
    ASSERT_TRUE(j);
    ASSERT_TRUE(j->is_int());
    ASSERT_EQ(j->as_int(), 123);
}

TEST(Parse, Float) {
    auto j = json::parse("123.456");
    ASSERT_TRUE(j);
    ASSERT_TRUE(j->is_float());
    ASSERT_FLOAT_EQ(j->as_float(), 123.456);
}

TEST(Parse, Exp) {
    auto j = json::parse("1e10");
    ASSERT_TRUE(j);
    ASSERT_TRUE(j->is_float());
    ASSERT_FLOAT_EQ(j->as_float(), 1e10);
}

TEST(Parse, FloatExp) {
    auto j = json::parse("1.2e10");
    ASSERT_TRUE(j);
    ASSERT_TRUE(j->is_float());
    ASSERT_FLOAT_EQ(j->as_float(), 1.2e10);
}

TEST(Parse, Str) {
    auto j = json::parse("\"hello\"");
    ASSERT_TRUE(j);
    ASSERT_TRUE(j->is_str());
    ASSERT_EQ(j->as_str(), "hello");
}
TEST(Parse, ObjEmpty) {
    auto j = json::parse("{}");
    ASSERT_TRUE(j);
    ASSERT_TRUE(j->is_obj());
    ASSERT_TRUE(j->as_obj().empty());
}

TEST(Parse, ObjStr) {
    auto j = json::parse(R"({"foo": "bar"})");
    ASSERT_TRUE(j);
    ASSERT_EQ(j->as_obj().at("foo").as_str(), "bar");
}

TEST(Parse, ObjInt) {
    auto j = json::parse(R"({"foo": 123})");
    ASSERT_TRUE(j);
    ASSERT_EQ(j->as_obj().at("foo").as_int(), 123);
}

TEST(Parse, ObjObj) {
    auto j = json::parse(R"({"foo": {"bar": "baz"}, "a": {"b": 1}})");
    ASSERT_TRUE(j);
    ASSERT_EQ(j->as_obj().at("foo").as_obj().at("bar").as_str(), "baz");
    ASSERT_EQ(j->as_obj().at("a").as_obj().at("b").as_int(), 1);
}

TEST(Parse, ArrEmpty) {
    auto j = json::parse("[]");
    ASSERT_TRUE(j);
    ASSERT_TRUE(j->is_arr());
    ASSERT_TRUE(j->as_arr().empty());
}

TEST(Parse, Arr) {
    auto j = json::parse("[1, 2, 3]");
    ASSERT_TRUE(j);
    ASSERT_TRUE(j->is_arr());
    ASSERT_EQ(j->as_arr().at(0).as_int(), 1);
    ASSERT_EQ(j->as_arr().at(1).as_int(), 2);
    ASSERT_EQ(j->as_arr().at(2).as_int(), 3);
}
