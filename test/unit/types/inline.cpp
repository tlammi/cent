#include <gtest/gtest.h>

#include <cent/types.hpp>

class Base {
 public:
    constexpr virtual ~Base() = default;

    virtual std::string_view name() const noexcept { return "Base"; }

 private:
};
class A : public Base {
 public:
    std::string_view name() const noexcept override { return "A"; }

 private:
};

class B : public Base {
 public:
    std::string_view name() const noexcept override { return "B"; }

 private:
};

TEST(Inline, MemberAccess) {
    auto a = cent::Inline<Base>(std::in_place_type<A>);
    auto b = cent::Inline<Base>(std::in_place_type<B>);
    ASSERT_EQ(a->name(), "A");
    ASSERT_EQ(b->name(), "B");
}

TEST(Inline, Dereference) {
    auto a = cent::Inline<Base>(std::in_place_type<A>);
    auto b = cent::Inline<Base>(std::in_place_type<B>);
    ASSERT_EQ((*a).name(), "A");
    ASSERT_EQ((*b).name(), "B");
}

TEST(Inline, ResourceCleanup) {
    int count = 0;
    class Derived : public Base {
     public:
        explicit Derived(int* i) : m_i(i) {}
        Derived(const Derived&) = delete;
        Derived(Derived&&) = delete;
        Derived& operator=(const Derived&) = delete;
        Derived& operator=(Derived&&) = delete;
        ~Derived() override { ++*m_i; }

     private:
        int* m_i;
    };

    {
        auto d = cent::Inline<Base>(std::in_place_type<Derived>, &count);
        ASSERT_EQ(count, 0);
    }
    ASSERT_EQ(count, 1);
}
