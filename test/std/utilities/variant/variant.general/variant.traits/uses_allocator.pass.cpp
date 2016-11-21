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

#include<variant>
#include<string>

using std::variant;
using std::uses_allocator_v;

int main()
{
  struct foo {};
  static_assert(uses_allocator_v<variant<bool>,std::allocator<char>>);
  static_assert(uses_allocator_v<variant<std::string,bool>,std::allocator<char>>);
  static_assert(uses_allocator_v<variant<foo>,std::allocator<char>>);
  static_assert(uses_allocator_v<variant<foo,bool>,std::allocator<char>>);
  static_assert(uses_allocator_v<variant<foo,std::string,bool>,std::allocator<char>>);
}
