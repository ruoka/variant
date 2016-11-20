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

#include<experimental/variant>
#include<string>
#include<iostream>

using std::experimental::variant;
using std::experimental::get;
using std::experimental::holds_alternative;

void test_copy_assign()
{
  variant<int,bool,std::string,double> v1, v2;

  v1 = 21.12;
  assert(get<double>(v1) == 21.12);
  assert(get<3>(v1)   == 21.12);

  v2 = v1;
  assert(get<double>(v2) == 21.12);
  assert(get<3>(v2)   == 21.12);
}

void test_move_assign()
{
  variant<int,bool,std::string,double> v;
  v = variant<int,bool,std::string,double>{std::string{"rush"}};

  assert(get<std::string>(v) == "rush");
  assert(get<2>(v)           == "rush");
}

void test_alternative_assign()
{
  variant<int,bool,std::string,double> v;

  v = 13;
  assert(get<int>(v) == 13);
  assert(get<0>(v)   == 13);

  v = false;
  assert(get<bool>(v) == false);
  assert(get<1>(v)    == false);

  v = std::string{"abc"};
  assert(get<std::string>(v) == "abc");
  assert(get<2>(v)           == "abc");

  v = 21.12;
  assert(get<double>(v) == 21.12);
  assert(get<3>(v)      == 21.12);
}

void test_pointer_assignent()
{
  auto v = variant<int*,bool*,std::string*,double*>{};
  auto i = 13;
  auto b = false;
  auto str = std::string{"TEST123"};
  auto d = 21.12;

  v = &i;
  assert(v.index() == 0);
  assert(holds_alternative<int*>(v));
  assert(*get<int*>(v) == i);
  assert(*get<0>(v)    == i);

  v = &b;
  assert(v.index() == 1);
  assert(holds_alternative<bool*>(v));
  assert(*get<bool*>(v) == b);
  assert(*get<1>(v)     == b);

  v = &str;
  assert(v.index() == 2);
  assert(holds_alternative<std::string*>(v));
  assert(*get<std::string*>(v) == str);
  assert(*get<2>(v)            == str);

  v = &d;
  assert(v.index() == 3);
  assert(holds_alternative<double*>(v));
  assert(*get<double*>(v) == d);
  assert(*get<3>(v)      == d);
}

int main()
{
  test_copy_assign();
  test_move_assign();
  test_alternative_assign();
  test_pointer_assignent();
}
