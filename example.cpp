#include <iostream>
#include <experimental/variant>

using std::experimental::variant;
using std::experimental::variant_size_v;
using std::experimental::variant_alternative_t;
using std::experimental::monostate;
using std::experimental::holds_alternative;
using std::experimental::get;
using std::experimental::visit;

struct test_1
{
    bool operator () (bool b1) const
    {
        std::cout << "bool" << std::endl;
        return true;
    }
    bool operator () (int i1) const
    {
        std::cout << "int" << std::endl;
        return true;
    }
    bool operator () (double d1) const
    {
        std::cout << "double" << std::endl;
        return true;
    }
    bool operator () (std::string str1) const
    {
        std::cout << "str" << std::endl;
        return true;
    }
};

struct test_2
{
    bool operator () (bool b1, bool b2) const
    {
        return true;
    }
    bool operator () (int i1, int i2) const
    {
        return true;
    }
    bool operator () (double d1, double d2) const
    {
        return true;
    }
    bool operator () (std::string str1, std::string str2) const
    {
        return true;
    }
};

template <typename T>
void test_3(T t)
{}

void test_4(bool b)
{}

class foo{};

int main()
{
    auto fd = variant<char*,int,bool,double>{2.1};
    auto fi = variant<bool,foo,double,int>{false};
    auto fs = variant<char,bool,double,std::string>{std::string{}};
    // auto ss = variant<std::string,std::string>{std::string{}};

    variant<monostate,void,bool,double> fb{};
    fb.emplace<bool>(true);

    using test = variant<bool,int,double,std::string>;
    std::cout << variant_size_v<test> << std::endl;

    variant_alternative_t<3,test> str = "abc";
    std::cout << str << std::endl;

    test v1{2.0};
    std::cout << v1.index() << std::endl;
    std::cout << get<2>(v1) << std::endl;

    std::cout << std::boolalpha << holds_alternative<bool>(v1) << std::endl;
    std::cout << std::boolalpha << holds_alternative<int>(v1) << std::endl;
    std::cout << std::boolalpha << holds_alternative<double>(v1) << std::endl;

    test v2{true};
    std::cout << v2.index() << std::endl;
    std::cout << get<0>(v2) << std::endl;

    std::cout << std::boolalpha << holds_alternative<bool>(v2) << std::endl;
    std::cout << std::boolalpha << holds_alternative<int>(v2) << std::endl;
    std::cout << std::boolalpha << holds_alternative<double>(v2) << std::endl;

    std::cout << std::boolalpha << (v1 == v2) << std::endl;

    v1 = false;

    std::cout << v1.index() << std::endl;
    std::cout << get<0>(v1) << std::endl;
    std::cout << std::boolalpha << holds_alternative<bool>(v1) << std::endl;
    std::cout << std::boolalpha << holds_alternative<int>(v1) << std::endl;
    std::cout << std::boolalpha << holds_alternative<double>(v1) << std::endl;

    std::cout << std::boolalpha << (v1 == v2) << std::endl;

    v1 = true;
    visit(test_1{}, v1);
    visit([](const auto& arg){std::cout << arg << std::endl;}, v1);
    v1 = 1;
    visit(test_1{}, v1);
    // visit([](const auto& arg){std::cout << arg << std::endl;}, v1);
    v1 = 2.2;
    visit(test_1{}, v1);
    // visit([](const auto& arg){std::cout << arg << std::endl;}, v1);

    v1 = false;
    visit(test_2{}, v1, v2);
    // visit([](const auto& arg1,const auto& arg2){std::cout << "2xarg" << std::endl;}, v1, v2);

    // visit(&test_3, v1);

    v1 = 78;

    auto hv = std::hash<test>{};
    std::cout << hv(v1) << std::endl;

    auto hb = std::hash<bool>{};
    std::cout << hb(true) << std::endl;

    std::allocator<char> a1;

    test v3{std::allocator_arg_t{}, a1, std::string{"ABCDEFG"}};
    // visit([](const auto& arg){std::cout << arg << std::endl;}, v3);

    test v4{std::allocator_arg_t{}, a1, std::string{"HIJKLM"}};
    // visit([](const auto& arg){std::cout << arg << std::endl;}, v4);

    constexpr variant<bool,int,double> v5{};

    constexpr variant<bool,int,double> v6{1};

    variant<bool,int,double,std::string> v7{true};

    test v10{std::string{"ABCDEFG"}};
    test v11{v10};

    test v12{std::allocator_arg_t{}, a1, v11};
    // visit([](const auto& arg){std::cout << arg << std::endl;}, v12);

    test v13{std::allocator_arg_t{}, a1, std::in_place<std::string>, "HIJKLM"};
    // visit([](const auto& arg){std::cout << arg << std::endl;}, v13);

    return 0;
}
