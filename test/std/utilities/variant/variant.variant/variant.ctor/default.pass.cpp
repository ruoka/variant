#include <variant>
#include <cassert>

struct foo
{
    foo() noexcept = default;
};

struct bar
{
    bar() = delete;
    ~bar() = default;
};

void test_type_traits()
{
    static_assert(std::is_default_constructible_v<std::variant<foo>>);
    static_assert(std::is_default_constructible_v<std::variant<int,bool,double,foo>>);
    static_assert(std::is_default_constructible_v<std::variant<foo,int,bool,double>>);

    static_assert(std::is_nothrow_default_constructible_v<std::variant<foo>>);
    static_assert(std::is_nothrow_default_constructible_v<std::variant<int,bool,double,foo>>);
    static_assert(std::is_nothrow_default_constructible_v<std::variant<foo,int,bool,double>>);

    static_assert(!std::is_default_constructible_v<std::variant<bar>>);
    static_assert(std::is_default_constructible_v<std::variant<int,bool,double,bar>>);
    static_assert(!std::is_default_constructible_v<std::variant<bar,int,bool,double>>);

    static_assert(!std::is_nothrow_default_constructible_v<std::variant<bar>>);
    static_assert(std::is_nothrow_default_constructible_v<std::variant<int,bool,double,bar>>);
    static_assert(!std::is_nothrow_default_constructible_v<std::variant<bar,int,bool,double>>);

    static_assert(std::is_nothrow_default_constructible_v<std::variant<foo,bar>>);
    static_assert(!std::is_nothrow_default_constructible_v<std::variant<bar,foo>>);
}

void test_constructors()
{
    constexpr std::variant<foo,int,bool,double> v1;
    static_assert(v1.index() == 0);
    static_assert(std::holds_alternative<foo>(v1));

    constexpr std::variant<int,foo,bar,bool,double> v2;
    static_assert(v2.index() == 0);
    static_assert(std::holds_alternative<int>(v2));
}

int main()
{
    test_type_traits();
    test_constructors();
}
