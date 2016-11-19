#include <variant>
#include <cassert>

using std::variant;

void test_comple_time_equal_to()
{
    using test = std::variant<bool,int,double>;
    constexpr test t1{1};
    constexpr test t2{1};
    constexpr test t3{true};
    static_assert(t1 == t2);
    static_assert(!(t1 != t2));
    static_assert(!(t1 == t3));
    static_assert(t2 != t3);
}

int main()
{
    test_comple_time_equal_to();
}
