// UNSUPPORTED: c++98, c++03, c++11, c++14

#include <variant>
#include <cassert>

using std::variant;
using std::holds_alternative;

struct foo
{
  constexpr foo(const foo&) noexcept = default;
};

struct foo2
{
  foo2(const foo2&) noexcept(false)
  {};
};

struct bar
{
  bar(const bar&) = delete;
};

void test_type_traits()
{
  static_assert(std::is_nothrow_copy_constructible_v<variant<foo>>);
  static_assert(std::is_nothrow_copy_constructible_v<variant<int,bool,double,foo>>);
  static_assert(std::is_nothrow_copy_constructible_v<variant<foo,int,bool,double>>);
  static_assert(std::is_nothrow_copy_constructible_v<variant<int,foo,bool,double>>);

  static_assert(std::is_copy_constructible_v<variant<foo,foo2>>);
  static_assert(std::is_copy_constructible_v<variant<int,bool,double,foo,foo2>>);
  static_assert(std::is_copy_constructible_v<variant<foo,foo2,int,bool,double>>);
  static_assert(std::is_copy_constructible_v<variant<int,foo,foo2,bool,double>>);

  static_assert(!std::is_nothrow_copy_constructible_v<variant<foo,foo2>>);
  static_assert(!std::is_nothrow_copy_constructible_v<variant<int,bool,double,foo,foo2>>);
  static_assert(!std::is_nothrow_copy_constructible_v<variant<foo,foo2,int,bool,double>>);
  static_assert(!std::is_nothrow_copy_constructible_v<variant<int,foo,foo2,bool,double>>);

  static_assert(!std::is_copy_constructible_v<variant<bar>>);
  static_assert(!std::is_copy_constructible_v<variant<int,bool,double,bar>>);
  static_assert(!std::is_copy_constructible_v<variant<bar,int,bool,double>>);
  static_assert(!std::is_copy_constructible_v<variant<int,bool,bar,double>>);
}

void test_constexpr()
{
  constexpr variant<int,bool,double,foo> v1{1.1};
  constexpr variant<int,bool,double,foo> v2{v1};
  static_assert(v1.index() == 2);
  static_assert(v2.index() == 2);
  static_assert(holds_alternative<double>(v1));
  static_assert(holds_alternative<double>(v2));
}

int main()
{
  test_type_traits();
  test_constexpr();
}
