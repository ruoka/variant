#include <experimental/variant>
#include <cassert>

using std::experimental::variant;

struct foo
{
    foo& operator = (const foo&) = default;
};

struct bar
{
    bar& operator = (const bar&) = delete;
};

int main()
{
    static_assert(std::is_copy_assignable_v<variant<foo>>);
    static_assert(std::is_copy_assignable_v<variant<int,bool,double,foo>>);
    static_assert(std::is_copy_assignable_v<variant<foo,int,bool,double>>);
    static_assert(std::is_copy_assignable_v<variant<int,foo,bool,double>>);

    static_assert(!std::is_copy_assignable_v<variant<bar>>);
    static_assert(!std::is_copy_assignable_v<variant<int,bool,double,bar>>);
    static_assert(!std::is_copy_assignable_v<variant<bar,int,bool,double>>);
    static_assert(!std::is_copy_assignable_v<variant<int,bool,bar,double>>);
}
