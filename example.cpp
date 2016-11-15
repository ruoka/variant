#include <variant>
#include <iostream>

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

int main()
{
    using variant = std::variant<bool,int,double,std::string>;

    std::cout << std::__helper::__index_v<bool,bool,int,double,std::string> << std::endl;
    std::cout << std::variant_size_v<variant> << std::endl;
    std::variant_alternative_t<3,variant> str = "abc";
    std::cout << str << std::endl;

    variant v1{2.0};
    std::cout << v1.index() << std::endl;
    std::cout << std::get<2>(v1) << std::endl;

    std::cout << std::boolalpha << std::holds_alternative<bool>(v1) << std::endl;
    std::cout << std::boolalpha << std::holds_alternative<int>(v1) << std::endl;
    std::cout << std::boolalpha << std::holds_alternative<double>(v1) << std::endl;

    variant v2{true};
    std::cout << v2.index() << std::endl;
    std::cout << std::get<0>(v2) << std::endl;

    std::cout << std::boolalpha << std::holds_alternative<bool>(v2) << std::endl;
    std::cout << std::boolalpha << std::holds_alternative<int>(v2) << std::endl;
    std::cout << std::boolalpha << std::holds_alternative<double>(v2) << std::endl;

    std::cout << std::boolalpha << (v1 == v2) << std::endl;

    v1 = false;

    std::cout << v1.index() << std::endl;
    std::cout << std::get<0>(v1) << std::endl;
    std::cout << std::boolalpha << std::holds_alternative<bool>(v1) << std::endl;
    std::cout << std::boolalpha << std::holds_alternative<int>(v1) << std::endl;
    std::cout << std::boolalpha << std::holds_alternative<double>(v1) << std::endl;

    std::cout << std::boolalpha << (v1 == v2) << std::endl;

    constexpr auto less = std::__helper::__make_less_array<bool,int,double,std::string>(std::index_sequence_for<bool, int, double, std::string>{});
    std::cout << std::boolalpha << less[0](v1,v2) << std::endl;

    v1 = true;
    std::visit(test_1{}, v1);
    std::visit([](const auto& arg){std::cout << arg << std::endl;}, v1);
    v1 = 1;
    std::visit(test_1{}, v1);
    std::visit([](const auto& arg){std::cout << arg << std::endl;}, v1);
    v1 = 2.2;
    std::visit(test_1{}, v1);
    std::visit([](const auto& arg){std::cout << arg << std::endl;}, v1);

    v1 = false;
    std::visit(test_2{}, v1, v2);
    std::visit([](const auto& arg1,const auto& arg2){std::cout << "2xarg" << std::endl;}, v1, v2);

    // std::visit(&test_3, v1);

    v1 = 78;

    auto hv = std::hash<variant>{};
    std::cout << hv(v1) << std::endl;

    auto hb = std::hash<bool>{};
    std::cout << hb(true) << std::endl;

    std::allocator<char> a1;
    variant v3{std::allocator_arg_t{}, a1, std::string{"ABCDEFG"}};
    std::visit([](const auto& arg){std::cout << arg << std::endl;}, v3);

    std::allocator<char> a2;
    variant v4{std::allocator_arg_t{}, a1, std::string{"HIJKLM"}};
    std::visit([](const auto& arg){std::cout << arg << std::endl;}, v4);

    constexpr std::variant<bool,int,double> v5{};

    constexpr std::variant<bool,int,double> v6{1};

    std::variant<bool,int,double,std::string> v7{true};

    variant v8{std::string{"ABCDEFG"}};
    variant v9{v8};

    return 0;
}
