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
using std::monostate;
using std::holds_alternative;

struct foo
{
  foo() = delete;
  foo(const foo&) = delete;
  foo(foo&&) = delete;
  foo(bool){}
  virtual ~foo(){}
};

int main()
{
  variant<monostate,foo> v;
  assert(holds_alternative<monostate>(v));
  v.emplace<foo>(true);
  assert(holds_alternative<foo>(v));
}
