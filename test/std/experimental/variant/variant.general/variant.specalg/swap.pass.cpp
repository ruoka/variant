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
#include <cassert>

using std::experimental::variant;
using std::experimental::get;
using std::experimental::swap;

struct foo{};

int main()
{
  variant<float,unsigned,double,char> v1, v2;
  v1 = 123456u;
  v2 = 'A';
  assert(get<1>(v1) == 123456u);
  assert(get<3>(v2) == 'A');
  swap<float,unsigned,double,char>(v1,v2);
  assert(get<1>(v2) == 123456u);
  assert(get<3>(v1) == 'A');
}
