#include <variant>
#include <cassert>

void test_comple_time_equal_to()
{
    using variant = std::variant<bool,int,double>;
    constexpr variant v1{1};
    constexpr variant v2{1};
    constexpr variant v3{true};
    static_assert(v1 == v2);
    static_assert(!(v1 != v2));
    static_assert(!(v1 == v3));
    static_assert(v2 != v3);
}

int main()
{
    test_comple_time_equal_to();
}
