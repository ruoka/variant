// -*- C++ -*-
//===-------------------------- variant ----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <experimental/variant>
#include <string>
#include <vector>

using std::experimental::variant;
using std::experimental::monostate;
using std::experimental::holds_alternative;

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
