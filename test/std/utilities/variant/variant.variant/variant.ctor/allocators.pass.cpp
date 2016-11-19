#include <variant>
#include <vector>
#include <string>
#include <cassert>

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
    static_assert(std::is_constructible_v<std::variant<int,foo>,  std::allocator_arg_t, std::allocator<char>, foo>);
    static_assert(!std::is_constructible_v<std::variant<int,bar>, std::allocator_arg_t, std::allocator<char>, bar>);

    // Copy
    static_assert(std::is_constructible_v<std::variant<int,foo>,  std::allocator_arg_t, std::allocator<char>, std::variant<int,foo>&>);
    static_assert(!std::is_constructible_v<std::variant<int,bar>, std::allocator_arg_t, std::allocator<char>, std::variant<int,bar>&>);

    // Move
    static_assert(std::is_constructible_v<std::variant<int,foo>,  std::allocator_arg_t, std::allocator<char>, std::variant<int,foo>&&>);
    static_assert(!std::is_constructible_v<std::variant<int,bar>, std::allocator_arg_t, const std::allocator<char>&, std::variant<int,bar>&&>);
}

void test_constructors()
{
    using variant = std::variant<bool,int,double,std::vector<int>,std::string>;
    std::allocator<char> a;

    variant v1{std::allocator_arg_t{}, a};
    assert(v1.index() == 0);

    variant v2{std::allocator_arg_t{}, a, 1};
    assert(v2.index() == 1);

    variant v3{std::allocator_arg_t{}, a, 2.2};
    assert(v3.index() == 2);

    variant v4{std::allocator_arg_t{}, a, std::in_place<std::vector<int>>, {1,2,3,4,5,6,7}};
    assert(v4.index() == 3);

    variant v5{std::allocator_arg_t{}, a, std::in_place<3>, {1,2,3,4,5,6,7}};
    assert(v5.index() == 3);

    variant v6{std::allocator_arg_t{}, a, std::in_place<std::string>, "test"};
    assert(v6.index() == 4);

    variant v7{std::allocator_arg_t{}, a, std::in_place<4>, "test"};
    assert(v7.index() == 4);

    variant v8{std::allocator_arg_t{}, a, std::in_place<std::string>, {'a','b','c','d'}};
    assert(v8.index() == 4);

    std::string str{'x',10, a};

    // variant v9{std::allocator_arg_t{}, a, std::in_place<4>, std::size_t{10}, 'a'};
    // assert(v9.index() == 4);
}

int main()
{
    test_type_traits();
    test_constructors();
}
