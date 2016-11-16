#include <variant>
#include <cassert>

struct foo
{
    foo(const foo&) = default;
};

struct bar
{
    bar(const bar&) = delete;
};

int main()
{
    static_assert(std::is_copy_constructible_v<std::variant<foo>>);
    static_assert(std::is_copy_constructible_v<std::variant<int,bool,double,foo>>);
    static_assert(std::is_copy_constructible_v<std::variant<foo,int,bool,double>>);
    static_assert(!std::is_copy_constructible_v<std::variant<bar>>);
    static_assert(std::is_copy_constructible_v<std::variant<int,bool,double,bar>>);
    static_assert(!std::is_copy_constructible_v<std::variant<bar,int,bool,double>>);
}
