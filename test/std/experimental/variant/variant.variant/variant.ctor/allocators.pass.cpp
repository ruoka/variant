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
#include <vector>
#include <string>
#include <cassert>

using std::experimental::variant;
using std::experimental::get;

struct foo
{
    foo(const foo&) = default;
    foo(foo&&) = default;
};

struct bar
{
    bar() = delete;
    bar(const bar&) = delete;
    bar(bar&&) = delete;
};

void test_type_traits()
{
    // Default
    static_assert(std::is_constructible_v<variant<int,foo>,  std::allocator_arg_t, std::allocator<char>, foo>);
    static_assert(!std::is_constructible_v<variant<int,bar>, std::allocator_arg_t, std::allocator<char>, bar>);

    // Copy
    static_assert(std::is_constructible_v<variant<int,foo>,  std::allocator_arg_t, std::allocator<char>, variant<int,foo>&>);
    static_assert(!std::is_constructible_v<variant<int,bar>, std::allocator_arg_t, std::allocator<char>, variant<int,bar>&>);

    // Move
    static_assert(std::is_constructible_v<variant<int,foo>,  std::allocator_arg_t, std::allocator<char>, variant<int,foo>&&>);
    static_assert(!std::is_constructible_v<variant<int,bar>, std::allocator_arg_t, const std::allocator<char>&, variant<int,bar>&&>);
}

void test_constructors()
{
    using variant = variant<bool,int,double,std::vector<int>,std::string>;
    std::allocator<char> a;

    variant v1{std::allocator_arg_t{}, a};
    assert(v1.index() == 0);
    assert(get<0>(v1) == false);

    variant v2{std::allocator_arg_t{}, a, 1};
    assert(v2.index() == 1);
    assert(get<1>(v2) == 1);

    variant v3{std::allocator_arg_t{}, a, 2.2};
    assert(v3.index() == 2);
    assert(get<2>(v3) == 2.2);

    variant v4{std::allocator_arg_t{}, a, std::in_place_type<std::vector<int>>, {1,2,3,4,5,6,7}};
    assert(v4.index() == 3);
    assert(get<3>(v4).size() == 7);

    variant v5{std::allocator_arg_t{}, a, std::in_place_index<3>, {1,2,3,4,5,6,7,8}};
    assert(v5.index() == 3);
    assert(get<3>(v5).size() == 8);

    variant v6{std::allocator_arg_t{}, a, std::in_place_type<std::string>, "foo"};
    assert(v6.index() == 4);
    assert(get<4>(v6) == "foo");

    variant v7{std::allocator_arg_t{}, a, std::in_place_index<4>, "bar"};
    assert(v7.index() == 4);
    assert(get<4>(v7) == "bar");

    variant v8{std::allocator_arg_t{}, a, std::in_place_type<std::string>, {'a','b','c','d'}};
    assert(v8.index() == 4);
    assert(get<4>(v8) == "abcd");

    variant v9{std::allocator_arg_t{}, a, std::in_place_index<4>, std::size_t{10}, 'a'};
    assert(v9.index() == 4);
    assert(get<4>(v9) == "aaaaaaaaaa");

    variant v22{std::allocator_arg_t{}, a, v2};
    assert(v22.index() == 1);
    assert(get<1>(v22) == 1);

    variant v23{std::allocator_arg_t{}, a, std::move(v3)};
    assert(v23.index() == 2);
    assert(get<2>(v23) == 2.2);
}

int main()
{
    test_type_traits();
    test_constructors();
}
