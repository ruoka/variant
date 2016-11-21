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
using std::get_if;

void test_constexpr_get()
{
  using test = variant<bool,int,double>;

  constexpr test t1{};
  static_assert(get<0>(t1)    == false);
  static_assert(get<bool>(t1) == false);

  constexpr test t2{true};
  static_assert(get<0>(t2)   == true);
  static_assert(get<bool>(t2) == true);

  constexpr test t3{11};
  static_assert(get<1>(t3)   == 11);
  static_assert(get<int>(t3) == 11);

  constexpr test t4{22.22};
  static_assert(get<2>(t4)      == 22.22);
  static_assert(get<double>(t4) == 22.22);

  static_assert(get<2>(test{3.3}) == 3.3);
}

void test_constexpr_get_if()
{
  using test = variant<bool,int,double>;

  constexpr test t1{};
  static_assert(get_if<0>(&t1)      != nullptr);
  static_assert(get_if<bool>(&t1)   != nullptr);
  static_assert(get_if<1>(&t1)      == nullptr);
  static_assert(get_if<int>(&t1)    == nullptr);
  static_assert(get_if<2>(&t1)      == nullptr);
  static_assert(get_if<double>(&t1) == nullptr);

  constexpr test t2{true};
  static_assert(*get_if<0>(&t2)      == true);
  static_assert(get_if<0>(&t2)      != nullptr);
  static_assert(get_if<bool>(&t2)   != nullptr);
  static_assert(get_if<1>(&t2)      == nullptr);
  static_assert(get_if<int>(&t2)    == nullptr);
  static_assert(get_if<2>(&t2)      == nullptr);
  static_assert(get_if<double>(&t2) == nullptr);

  constexpr test t3{11};
  static_assert(*get_if<1>(&t3)     == 11);
  static_assert(get_if<0>(&t3)      == nullptr);
  static_assert(get_if<bool>(&t3)   == nullptr);
  static_assert(get_if<1>(&t3)      != nullptr);
  static_assert(get_if<int>(&t3)    != nullptr);
  static_assert(get_if<2>(&t3)      == nullptr);
  static_assert(get_if<double>(&t3) == nullptr);

  constexpr test t4{22.22};
  static_assert(*get_if<2>(&t4)     == 22.22);
  static_assert(get_if<0>(&t4)      == nullptr);
  static_assert(get_if<bool>(&t4)   == nullptr);
  static_assert(get_if<1>(&t4)      == nullptr);
  static_assert(get_if<int>(&t4)    == nullptr);
  static_assert(get_if<2>(&t4)      != nullptr);
  static_assert(get_if<double>(&t4) != nullptr);
}

void test_get()
{
  using test = variant<bool,int,double,std::string>;

  test t1{false};
  get<0>(t1) = true;
  assert(get<0>(t1) == true);

  test t2{std::string{}};
  get<3>(t2) = "ABCDEFG";
  assert(get<3>(t2) == "ABCDEFG");

  const test t3{1};
  assert(get<1>(t3) == 1);
}

void test_get_if()
{
  using test = variant<bool,int,double,std::string>;

  test t1{false};
  *get_if<0>(&t1) = true;
  assert(*get_if<0>(&t1) == true);

  test t2{std::string{}};
  *get_if<3>(&t2) = "ABCDEFG";
  assert(*get_if<3>(&t2) == "ABCDEFG");

  const test t3{1};
  assert(*get_if<1>(&t3) == 1);
}

void test_get_odds()
{
  auto vola1 = variant<bool,bool,volatile int>{33};
  assert(get<volatile int>(vola1) == 33);

  variant<bool,const bool,const std::string> vola2{std::string{"Variant"}};
  assert(get<const std::string>(vola2) == std::string{"Variant"});
  assert(get<2>(vola2) == std::string{"Variant"});

  auto vola3 = variant<bool,const bool,const volatile int>{44};
  assert(get<const volatile int>(vola3) == 44);
  assert(get<2>(vola3) == 44);
}

int main()
{
  test_constexpr_get();
  test_constexpr_get_if();
  test_get();
  test_get_if();
  test_get_odds();
}
