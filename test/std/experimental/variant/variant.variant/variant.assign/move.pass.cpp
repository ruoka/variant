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
  foo(foo&&) = default;
  foo& operator = (foo&&) = default;
};

struct bar
{
  bar& operator = (bar&&) = delete;
};

int main()
{
  static_assert(std::is_move_assignable_v<variant<foo>>);
  static_assert(std::is_move_assignable_v<variant<int,bool,double,foo>>);
  static_assert(std::is_move_assignable_v<variant<foo,int,bool,double>>);
  static_assert(std::is_move_assignable_v<variant<int,foo,bool,double>>);

  static_assert(!std::is_move_assignable_v<variant<bar>>);
  static_assert(!std::is_move_assignable_v<variant<int,bool,double,bar>>);
  static_assert(!std::is_move_assignable_v<variant<bar,int,bool,double>>);
  static_assert(!std::is_move_assignable_v<variant<int,bool,bar,double>>);
}
