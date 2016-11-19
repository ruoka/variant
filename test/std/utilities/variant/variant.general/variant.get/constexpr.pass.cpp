#include <variant>
#include <string>
#include <cassert>

void test_constexpr_get()
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

    static_assert(std::get<2>(variant{3.3}) == 3.3);
}

void test_constexpr_get_if()
{
    using variant = std::variant<bool,int,double>;

    constexpr variant v1{};
    static_assert(std::get_if<0>(&v1)      != nullptr);
    static_assert(std::get_if<bool>(&v1)   != nullptr);
    static_assert(std::get_if<1>(&v1)      == nullptr);
    static_assert(std::get_if<int>(&v1)    == nullptr);
    static_assert(std::get_if<2>(&v1)      == nullptr);
    static_assert(std::get_if<double>(&v1) == nullptr);

    constexpr variant v2{true};
    static_assert(*std::get_if<0>(&v2)      == true);
    static_assert(std::get_if<0>(&v2)      != nullptr);
    static_assert(std::get_if<bool>(&v2)   != nullptr);
    static_assert(std::get_if<1>(&v2)      == nullptr);
    static_assert(std::get_if<int>(&v2)    == nullptr);
    static_assert(std::get_if<2>(&v2)      == nullptr);
    static_assert(std::get_if<double>(&v2) == nullptr);

    constexpr variant v3{11};
    static_assert(*std::get_if<1>(&v3)     == 11);
    static_assert(std::get_if<0>(&v3)      == nullptr);
    static_assert(std::get_if<bool>(&v3)   == nullptr);
    static_assert(std::get_if<1>(&v3)      != nullptr);
    static_assert(std::get_if<int>(&v3)    != nullptr);
    static_assert(std::get_if<2>(&v3)      == nullptr);
    static_assert(std::get_if<double>(&v3) == nullptr);

    constexpr variant v4{22.22};
    static_assert(*std::get_if<2>(&v4)     == 22.22);
    static_assert(std::get_if<0>(&v4)      == nullptr);
    static_assert(std::get_if<bool>(&v4)   == nullptr);
    static_assert(std::get_if<1>(&v4)      == nullptr);
    static_assert(std::get_if<int>(&v4)    == nullptr);
    static_assert(std::get_if<2>(&v4)      != nullptr);
    static_assert(std::get_if<double>(&v4) != nullptr);
}

void test_get()
{
    using variant = std::variant<bool,int,double,std::string>;

    variant v1{false};
    std::get<0>(v1) = true;
    assert(std::get<0>(v1) == true);

    variant v2{};
    std::get<3>(v2) = "ABCDEFG";
    assert(std::get<3>(v2) == "ABCDEFG");

    const variant v3{1};
    assert(std::get<1>(v3) == 3);
}

void test_get_if()
{
    using variant = std::variant<bool,int,double,std::string>;

    variant v1{false};
    *std::get_if<0>(&v1) = true;
    assert(*std::get_if<0>(&v1) == true);

    variant v2{};
    *std::get_if<3>(&v2) = "ABCDEFG";
    assert(*std::get_if<3>(&v2) == "ABCDEFG");

    const variant v3{1};
    assert(*std::get_if<1>(&v3) == 3);
}

int main()
{
    test_constexpr_get();
    test_constexpr_get_if();
    test_get();
    test_get_if();
}
