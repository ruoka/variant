#include <variant>
#include <string>
#include <cassert>

struct foo
{
    foo() noexcept = default;
    foo(const foo&) noexcept = default;
    foo(foo&&) noexcept = default;
};

struct bar
{
    bar() = delete;
    bar(const bar&) = delete;
    bar(bar&&) = delete;
};

int main()
{
    // Well formed
    static_assert(std::is_nothrow_constructible_v<std::variant<foo>,foo>);
    static_assert(std::is_nothrow_constructible_v<std::variant<int,bool,double,foo>,int>);
    static_assert(std::is_nothrow_constructible_v<std::variant<foo,int,bool,double>,bool>);
    static_assert(!std::is_nothrow_constructible_v<std::variant<bar>,bar>);
    static_assert(std::is_nothrow_constructible_v<std::variant<int,bool,double,bar>,double>);
    static_assert(!std::is_nothrow_constructible_v<std::variant<bar,int,bool,double>,bar>);
    // Ill formed
    static_assert(!std::is_nothrow_constructible_v<std::variant<bool,bool>,bool>);
    static_assert(!std::is_nothrow_constructible_v<std::variant<std::string,std::string>,const char*>);
}
