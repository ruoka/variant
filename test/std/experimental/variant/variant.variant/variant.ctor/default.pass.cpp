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

struct foo
{
    foo() noexcept = default;
};

struct bar
{
    bar() = delete;
    ~bar() = default;
};

void test_type_traits()
{
    static_assert(std::is_default_constructible_v<variant<foo>>);
    static_assert(std::is_default_constructible_v<variant<int,bool,double,foo>>);
    static_assert(std::is_default_constructible_v<variant<foo,int,bool,double>>);

    static_assert(std::is_nothrow_default_constructible_v<variant<foo>>);
    static_assert(std::is_nothrow_default_constructible_v<variant<int,bool,double,foo>>);
    static_assert(std::is_nothrow_default_constructible_v<variant<foo,int,bool,double>>);

    static_assert(!std::is_default_constructible_v<variant<bar>>);
    static_assert(std::is_default_constructible_v<variant<int,bool,double,bar>>);
    static_assert(!std::is_default_constructible_v<variant<bar,int,bool,double>>);

    static_assert(!std::is_nothrow_default_constructible_v<variant<bar>>);
    static_assert(std::is_nothrow_default_constructible_v<variant<int,bool,double,bar>>);
    static_assert(!std::is_nothrow_default_constructible_v<variant<bar,int,bool,double>>);

    static_assert(std::is_nothrow_default_constructible_v<variant<foo,bar>>);
    static_assert(!std::is_nothrow_default_constructible_v<variant<bar,foo>>);
}

void test_constructors()
{
    constexpr variant<foo,int,bool,double> v1;
    static_assert(v1.index() == 0);
    static_assert(holds_alternative<foo>(v1));

    constexpr variant<int,foo,bar,bool,double> v2;
    static_assert(v2.index() == 0);
    static_assert(holds_alternative<int>(v2));
}

int main()
{
    test_type_traits();
    test_constructors();
}
