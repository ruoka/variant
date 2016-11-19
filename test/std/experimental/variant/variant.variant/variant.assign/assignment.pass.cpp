// -*- C++ -*-
//===-------------------------- variant ----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include<experimental/variant>
#include<string>
#include<iostream>

using std::experimental::variant;
using std::experimental::get;

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

int main()
{
    test_copy_assign();
    test_move_assign();
    test_alternative_assign();
}
