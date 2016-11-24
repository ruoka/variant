// UNSUPPORTED: c++98, c++03, c++11, c++14

#include <variant>
#include <string>
#include <cassert>

using std::variant;
using std::holds_alternative;
using std::variant_npos;
using std::get;

struct foo
{
  foo(foo&&) noexcept = default;
};

struct foo2
{
  foo2(foo2&&) noexcept(false)
  {};
};

struct bar
{
  bar(bar&&) = delete;
};

void test_type_traits()
{
  static_assert(std::is_nothrow_move_constructible_v<variant<foo>>);
  static_assert(std::is_nothrow_move_constructible_v<variant<int,bool,double,foo>>);
  static_assert(std::is_nothrow_move_constructible_v<variant<foo,int,bool,double>>);
  static_assert(std::is_nothrow_move_constructible_v<variant<int,foo,bool,double>>);

  static_assert(std::is_move_constructible_v<variant<foo,foo2>>);
  static_assert(std::is_move_constructible_v<variant<int,bool,double,foo,foo2>>);
  static_assert(std::is_move_constructible_v<variant<foo,foo2,int,bool,double>>);
  static_assert(std::is_move_constructible_v<variant<int,foo,foo2,bool,double>>);

  static_assert(!std::is_nothrow_move_constructible_v<variant<foo,foo2>>);
  static_assert(!std::is_nothrow_move_constructible_v<variant<int,bool,double,foo,foo2>>);
  static_assert(!std::is_nothrow_move_constructible_v<variant<foo,foo2,int,bool,double>>);
  static_assert(!std::is_nothrow_move_constructible_v<variant<int,foo,foo2,bool,double>>);

  static_assert(!std::is_move_constructible_v<variant<bar>>);
  static_assert(!std::is_move_constructible_v<variant<int,bool,double,bar>>);
  static_assert(!std::is_move_constructible_v<variant<bar,int,bool,double>>);
  static_assert(!std::is_move_constructible_v<variant<int,bool,bar,double>>);
}

void test_constexpr()
{
  constexpr variant<int,bool,double,foo> v{variant<int,bool,double,foo>{1.1}};
  static_assert(v.index() == 2);
  static_assert(holds_alternative<double>(v));
  static_assert(get<double>(v) == 1.1);
}

void test_constructors()
{
  auto v1 = variant<int,bool,double,std::string>{2.2};
  assert(v1.index() == 2);
  assert(holds_alternative<double>(v1));
  assert(get<double>(v1) == 2.2);

  auto v2 = variant<int,bool,double,std::string>{std::move(v1)};
  assert(v2.index() == 2);
  assert(holds_alternative<double>(v2));
  assert(get<double>(v2) == 2.2);

  assert(v1.index() == variant_npos);

  auto vola = variant<bool,bool,volatile int>{33};
  assert(vola.index() == 2);
  assert(holds_alternative<volatile int>(vola));
  assert(get<volatile int>(vola) == 33);
}

int main()
{
  test_type_traits();
  test_constexpr();
  test_constructors();
}
