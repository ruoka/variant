#include <experimental/variant>
#include <string>
#include <cassert>

using std::experimental::variant;

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

void test_type_traits()
{
    // Well formed
    static_assert(std::is_nothrow_constructible_v<variant<foo>,foo>);
    static_assert(std::is_nothrow_constructible_v<variant<int,bool,double,foo>,int>);
    static_assert(std::is_nothrow_constructible_v<variant<foo,int,bool,double>,bool>);

    static_assert(!std::is_nothrow_constructible_v<variant<bar>,bar>);
    static_assert(std::is_nothrow_constructible_v<variant<int,bool,double,bar>,double>);
    static_assert(!std::is_nothrow_constructible_v<variant<bar,int,bool,double>,bar>);

    static_assert(std::is_constructible_v<variant<std::string,foo>,std::string>);
    static_assert(std::is_constructible_v<variant<std::string,foo>,const std::string&>);
    static_assert(std::is_constructible_v<variant<std::string,foo>,std::string&&>);
    static_assert(std::is_constructible_v<variant<std::string,foo>,char*>);
    static_assert(std::is_constructible_v<variant<std::string,foo>,const char*>);

    // Ill formed
    static_assert(!std::is_nothrow_constructible_v<variant<bool,bool>,bool>);
    static_assert(!std::is_nothrow_constructible_v<variant<std::string,std::string>,const char*>);
}

int main()
{
    test_type_traits();
}
