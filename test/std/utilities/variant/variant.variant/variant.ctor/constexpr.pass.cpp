#include <variant>
#include <vector>
#include <array>
#include <cassert>

void test_constexpr()
{
    using variant = std::variant<bool,int,double>;
    constexpr variant v1{};
    static_assert(v1.index() == 0);
}

void test_all()
{
    using variant = std::variant<bool,int,double,std::vector<int>>;
    variant v1{};
    variant v2{true};
    variant v3{v2};
    // variant v4{variant{2.2}};
    variant v5{std::in_place<int>, 13};

    std::allocator<char> a;

    variant v6{std::allocator_arg_t{}, a, 1};

    variant v7{std::allocator_arg_t{}, a, std::in_place<std::vector<int>>, {1,2,3,4,5,6,7}};

    variant v8{std::allocator_arg_t{}, a, std::in_place<3>, {1,2,3,4,5,6,7}};

    variant v9{std::in_place<3>, {1,2,3,4,5,6,7}};

    // using variant2 = std::variant<bool,int,double,std::array<int,7>>;
    // variant2 v10{std::in_place<std::array<int,7>>, {1,2,3,4,5,6,7}}; // FIXME
    // variant2 v10{std::allocator_arg_t{}, a, std::in_place<3>, {1,2,3,4,5,6,7}}; // FIXME
}

int main()
{
    test_constexpr();
    test_all();
}
