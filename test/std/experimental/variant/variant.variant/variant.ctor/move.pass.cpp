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
using std::experimental::holds_alternative;
using std::experimental::variant_npos;

struct foo
{
    foo(foo&&) = default;
};

struct bar
{
    bar(bar&&) = delete;
};

void test_type_traits()
{
    static_assert(std::is_move_constructible_v<variant<foo>>);
    static_assert(std::is_move_constructible_v<variant<int,bool,double,foo>>);
    static_assert(std::is_move_constructible_v<variant<foo,int,bool,double>>);
    static_assert(std::is_move_constructible_v<variant<int,foo,bool,double>>);

    static_assert(!std::is_move_constructible_v<variant<bar>>);
    static_assert(!std::is_move_constructible_v<variant<int,bool,double,bar>>);
    static_assert(!std::is_move_constructible_v<variant<bar,int,bool,double>>);
    static_assert(!std::is_move_constructible_v<variant<int,bool,bar,double>>);
}

void test_constructors()
{
    variant<int,bool,double> v1{2.2};
    assert(v1.index() == 2);
    assert(holds_alternative<double>(v1));

    variant<int,bool,double> v2{std::move(v1)};
    assert(v2.index() == 2);
    assert(holds_alternative<double>(v2));

    assert(v1.index() == variant_npos);
}

int main()
{
    test_type_traits();
    test_constructors();
}
