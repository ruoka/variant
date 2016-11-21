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
#include <cassert>

using std::variant;
using std::bad_variant_access;
using std::variant_npos;

void test_ctor()
{
  using test = variant<bool,int,double>;
  constexpr test t1{};
  static_assert(t1.index() == 0);
  constexpr test t2{true};
  static_assert(t2.index() == 0);
  constexpr test t3{1};
  static_assert(t3.index() == 1);
  constexpr test t4{2.2};
  static_assert(t4.index() == 2);
}

void test_assign()
{
  using test = variant<bool,int,double>;
  test t1{};
  assert(t1.index() == 0);
  t1 = true;
  assert(t1.index() == 0);
  t1 = 1;
  assert(t1.index() == 1);
  t1 = 2.2;
  assert(t1.index() == 2);
}

void test_valueless_by_exception()
{
  struct S { operator int() { throw 42; }};
  variant<float, int> v{12.f};
  assert(v.index() == 0);
  assert(!v.valueless_by_exception());
  try
  {
    v.emplace<1>(S());
  }
  catch(...)
  {}
  assert(v.index() == variant_npos);
  assert(v.valueless_by_exception());
}

int main()
{
  test_ctor();
  test_assign();
  test_valueless_by_exception();
}
