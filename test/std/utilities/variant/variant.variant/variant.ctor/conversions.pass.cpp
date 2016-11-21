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

#include <variant>
#include <string>
#include <cassert>

using std::variant;
using std::get;

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

void test_odds()
{
  auto i = 13;
  variant<bool*,int&,double*,std::string&> odd1{std::in_place_type<int&>, i};
  assert(odd1.index() == 1);
  assert(get<1>(odd1) == i);

  const auto str = std::string{"Jee!"};
  variant<bool*,int&,double*,const std::string&> odd2{std::in_place_type<const std::string&>, str};
  assert(odd2.index() == 3);
  assert(get<3>(odd2) == str);

  auto b = false;
  variant<bool*,int&,double*,const std::string&> odd3{std::in_place_type<bool*>, &b};
  assert(odd3.index() == 0);
  assert(*get<0>(odd3) == b);
}

int main()
{
  test_type_traits();
  test_odds();
}
