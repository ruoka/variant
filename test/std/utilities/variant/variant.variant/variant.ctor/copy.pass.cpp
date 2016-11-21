// UNSUPPORTED: c++98, c++03, c++11, c++14

#include <variant>
#include <cassert>

using std::variant;

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
  static_assert(std::is_copy_constructible_v<variant<foo>>);
  static_assert(std::is_copy_constructible_v<variant<int,bool,double,foo>>);
  static_assert(std::is_copy_constructible_v<variant<foo,int,bool,double>>);
  static_assert(std::is_copy_constructible_v<variant<int,foo,bool,double>>);

  static_assert(!std::is_copy_constructible_v<variant<bar>>);
  static_assert(!std::is_copy_constructible_v<variant<int,bool,double,bar>>);
  static_assert(!std::is_copy_constructible_v<variant<bar,int,bool,double>>);
  static_assert(!std::is_copy_constructible_v<variant<int,bool,bar,double>>);
}
