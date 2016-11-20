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
using std::experimental::holds_alternative;
using std::experimental::variant_npos;

struct foo
{
  foo(foo&&) = default;
};

struct bar
{
  bar(bar&&) = delete;
};

void test_type_traits()
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

void test_constructors()
{
  auto v1 = variant<int,bool,double,std::string>{2.2};
  assert(v1.index() == 2);
  assert(holds_alternative<double>(v1));

  auto v2 = variant<int,bool,double,std::string>{std::move(v1)};
  assert(v2.index() == 2);
  assert(holds_alternative<double>(v2));

  assert(v1.index() == variant_npos);

  auto vola = variant<bool,bool,volatile int>{33};
  assert(vola.index() == 2);
  assert(holds_alternative<volatile int>(vola));
}

int main()
{
  test_type_traits();
  test_constructors();
}
