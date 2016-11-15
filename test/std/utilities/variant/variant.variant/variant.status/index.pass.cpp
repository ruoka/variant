#include <variant>
#include <cassert>

void test_constexpr()
{
    using variant = std::variant<bool,int,double>;

    constexpr variant v1{};
    static_assert(v1.index() == 0);

    constexpr variant v2{true};
    static_assert(v2.index() == 0);

    constexpr variant v3{1};
    static_assert(v3.index() == 1);

    constexpr variant v4{2.2};
    static_assert(v4.index() == 2);
}

int main()
{
    test_constexpr();
}
