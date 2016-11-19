#include <experimental/variant>
#include <cassert>

using std::experimental::variant;

struct foo
{
    foo(foo&&) = default;
};

struct bar
{
    bar(bar&&) = delete;
};

int main()
{
    static_assert(std::is_move_constructible_v<variant<foo>>);
    static_assert(std::is_move_constructible_v<variant<int,bool,double,foo>>);
    static_assert(std::is_move_constructible_v<variant<foo,int,bool,double>>);
    static_assert(std::is_move_constructible_v<variant<int,foo,bool,double>>);

    static_assert(!std::is_move_constructible_v<variant<bar>>);
    static_assert(!std::is_move_constructible_v<variant<int,bool,double,bar>>);
    static_assert(!std::is_move_constructible_v<variant<bar,int,bool,double>>);
    static_assert(!std::is_move_constructible_v<variant<int,bool,bar,double>>);
}
