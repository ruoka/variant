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

using std::experimental::variant;
using std::experimental::get;

static int count = 0;

struct foo
{
  foo(int) {++count;} // count == 1
  ~foo(){++count;} // count == 2
};

void test_basics()
{
  variant<bool,foo> v{};
  assert(count == 0);
  v.emplace<foo>(13);
  assert(count == 1);
  v.emplace<bool>(true);
  assert(count == 2);
  v.emplace<foo>(13);
  assert(count == 3);
  v.emplace<bool>(true);
  assert(count == 4);
}

void test_odds()
{
  auto b = false;
  variant<bool*,int&,double*,const std::string&> odd3{};
  odd3.emplace<0>(&b);
  assert(odd3.index() == 0);
  assert(*get<0>(odd3) == b);
}

int main()
{
  test_basics();
  test_odds();
}
