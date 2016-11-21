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
#include <cassert>

using std::experimental::variant;

struct foo
{
  foo(const foo&) noexcept = default;
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

int main()
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
