#include <gtest/gtest.h>

#include <cent/dt.hpp>

struct Foo {
    int a;
    int b;
};

struct Bar {
    std::string a;
    std::string b;
};

TEST(Ctor, Default) {
    cent::Result<int> r{};
    ASSERT_TRUE(r);
}

TEST(Ctor, EmplaceVal) {
    auto r = cent::Result<Foo>(cent::value, 1, 2);
    ASSERT_TRUE(r);
    ASSERT_EQ(r->a, 1);
    ASSERT_EQ(r->b, 2);
}

TEST(Ctor, EmplaceErr) {
    auto r = cent::BasicResult<int, Foo>(cent::error, 1, 2);
    ASSERT_FALSE(r);
    ASSERT_EQ(r.error().a, 1);
    ASSERT_EQ(r.error().b, 2);
}

TEST(Ctor, ImplicitValue) {
    auto mk_result = []() -> cent::BasicResult<Foo, Bar> { return Foo{1, 2}; };
    auto r = mk_result();
    ASSERT_TRUE(r);
    ASSERT_EQ(r->a, 1);
    ASSERT_EQ(r->b, 2);
}

TEST(Ctor, ImplicitErr) {
    auto mk_err = []() -> cent::BasicResult<Foo, Bar> { return Bar{"a", "b"}; };
    auto r = mk_err();
    ASSERT_FALSE(r);
    ASSERT_EQ(r.error().a, "a");
    ASSERT_EQ(r.error().b, "b");
}

TEST(Dtor, Value) {
    size_t counter = 0;

    struct Inc {
        Inc(size_t* counter) : c(counter) {}
        Inc(Inc&) = delete;
        Inc& operator=(Inc&) = delete;

        Inc(Inc&&) = delete;
        Inc& operator=(Inc&&) = delete;
        ~Inc() { ++(*c); }

        size_t* c;
    };
    { auto r = cent::Result<Inc>(&counter); }
    ASSERT_EQ(counter, 1);
}

TEST(Dtor, Error) {
    size_t counter = 0;

    struct Inc {
        Inc(size_t* counter) : c(counter) {}
        Inc(Inc&) = delete;
        Inc& operator=(Inc&) = delete;

        Inc(Inc&&) = delete;
        Inc& operator=(Inc&&) = delete;
        ~Inc() { ++(*c); }

        size_t* c;
    };
    { auto r = cent::BasicResult<int, Inc>(&counter); }
    ASSERT_EQ(counter, 1);
}

