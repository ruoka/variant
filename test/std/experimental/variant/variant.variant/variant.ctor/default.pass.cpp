// -*- C++ -*-
//===-------------------------- variant ----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14

#include <experimental/variant>
#include <string>
#include <cassert>

using std::experimental::variant;
using std::experimental::monostate;
using std::experimental::holds_alternative;
using std::experimental::get;

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
  static_assert(std::is_default_constructible_v<variant<foo>>);
  static_assert(std::is_default_constructible_v<variant<int,bool,double,foo>>);
  static_assert(std::is_default_constructible_v<variant<foo,int,bool,double>>);

  static_assert(std::is_nothrow_default_constructible_v<variant<foo>>);
  static_assert(std::is_nothrow_default_constructible_v<variant<int,bool,double,foo>>);
  static_assert(std::is_nothrow_default_constructible_v<variant<foo,int,bool,double>>);

  static_assert(!std::is_default_constructible_v<variant<bar>>);
  static_assert(std::is_default_constructible_v<variant<int,bool,double,bar>>);
  static_assert(!std::is_default_constructible_v<variant<bar,int,bool,double>>);

  static_assert(!std::is_nothrow_default_constructible_v<variant<bar>>);
  static_assert(std::is_nothrow_default_constructible_v<variant<int,bool,double,bar>>);
  static_assert(!std::is_nothrow_default_constructible_v<variant<bar,int,bool,double>>);

  static_assert(std::is_nothrow_default_constructible_v<variant<foo,bar>>);
  static_assert(!std::is_nothrow_default_constructible_v<variant<bar,foo>>);
}

void test_constructors()
{
  constexpr variant<foo,int,bool,double> v1;
  static_assert(v1.index() == 0);
  static_assert(holds_alternative<foo>(v1));

  constexpr variant<int,foo,bar,bool,double> v2;
  static_assert(v2.index() == 0);
  static_assert(holds_alternative<int>(v2));
}

void test_odds()
{
  auto i = 13;
  variant<bool*,int&,double*,std::string&> odd1{i};
  assert(holds_alternative<int&>(odd1));
  assert(!holds_alternative<const std::string&>(odd1));
  assert(!holds_alternative<const std::string>(odd1));
  assert(!holds_alternative<std::string>(odd1));
  assert(!holds_alternative<std::string&>(odd1));
  assert(!holds_alternative<std::nullptr_t>(odd1));
  assert(odd1.index() == 1);
  assert(get<1>(odd1) == i);

  const auto str = std::string{"Jee!"};
  variant<bool*,int&,double*,const std::string&> odd2{str};
  assert(holds_alternative<const std::string&>(odd2));
  assert(!holds_alternative<const std::string>(odd2));
  assert(!holds_alternative<std::string>(odd2));
  assert(!holds_alternative<std::string&>(odd2));
  assert(!holds_alternative<int&>(odd2));
  assert(odd2.index() == 3);
  assert(get<3>(odd2) == str);

  auto b = false;
  variant<bool*,int&,double*,const std::string&> odd3{&b};
  assert(holds_alternative<bool*>(odd3));
  assert(!holds_alternative<int&>(odd3));
  assert(!holds_alternative<unsigned>(odd3));
  assert(odd3.index() == 0);
  assert(*get<0>(odd3) == b);

  variant<const volatile int> vola{33};
  assert(holds_alternative<const volatile int>(vola));
  assert(!holds_alternative<volatile int>(vola));
  assert(!holds_alternative<int>(vola));
  assert(!holds_alternative<const int>(vola));
  assert(!holds_alternative<bool>(vola));
  assert(vola.index() == 0);
  assert(get<0>(vola) == 33);
}

int main()
{
  test_type_traits();
  test_constructors();
  test_odds();
}
