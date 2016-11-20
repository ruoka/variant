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
#include <string>
#include <vector>

using std::experimental::variant;
using std::experimental::variant_size_v;
using std::experimental::variant_alternative_t;

int main()
{
  using V = variant<unsigned, double, char, float, std::string, std::vector<int>>;
  static_assert(variant_size_v<V> == 6);
  static_assert(std::is_same_v<variant_alternative_t<5, V>, std::vector<int>>);
}
