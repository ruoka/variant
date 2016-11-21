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
using std::get;
using std::bad_variant_access;

int main()
{
  try
  {
    variant<unsigned,double,char> v{9u};
    get<char>(v);
    assert(false);
  }
  catch(bad_variant_access& bad)
  {}

    try
    {
      variant<unsigned,double,char> v{9u};
      get<2>(v);
      assert(false);
    }
    catch(bad_variant_access& bad)
    {}

      try
      {
        variant<unsigned,double,char> v{9u};
        get<unsigned>(v);
      }
      catch(bad_variant_access& bad)
      {
        assert(false);
      }

      try
      {
        variant<unsigned,double,char> v{9u};
        get<0>(v);
      }
      catch(bad_variant_access& bad)
      {
        assert(false);
      }
    }
