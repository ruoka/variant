#include <variant>
#include <cassert>

void test_constexpr()
{
    using variant = std::variant<bool,int,double>;

    constexpr variant v1{};
    static_assert(std::get<0>(v1)    == false);
    static_assert(std::get<bool>(v1) == false);

    constexpr variant v2{true};
    static_assert(std::get<0>(v2)   == true);
    static_assert(std::get<bool>(v2) == true);

    constexpr variant v3{11};
    static_assert(std::get<1>(v3)   == 11);
    static_assert(std::get<int>(v3) == 11);

    constexpr variant v4{22.22};
    static_assert(std::get<2>(v4)      == 22.22);
    static_assert(std::get<double>(v4) == 22.22);
}

int main()
{
    test_constexpr();
}
