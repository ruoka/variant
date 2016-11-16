#include<variant>
#include<string>
#include<iostream>

void test_copy_assign()
{
    std::variant<int,bool,std::string,double> v1, v2;

    v1 = 21.12;
    assert(std::get<double>(v1) == 21.12);
    assert(std::get<3>(v1)   == 21.12);

    v2 = v1;
    assert(std::get<double>(v2) == 21.12);
    assert(std::get<3>(v2)   == 21.12);
}

void test_move_assign()
{
    std::variant<int,bool,std::string,double> v;
    v = std::variant<int,bool,std::string,double>{std::string{"rush"}};

    assert(std::get<std::string>(v) == "rush");
    assert(std::get<2>(v)           == "rush");
}

void test_alternative_assign()
{
    std::variant<int,bool,std::string,double> v;

    v = 13;
    assert(std::get<int>(v) == 13);
    assert(std::get<0>(v)   == 13);

    v = false;
    assert(std::get<bool>(v) == false);
    assert(std::get<1>(v)    == false);

    v = std::string{"abc"};
    assert(std::get<std::string>(v) == "abc");
    assert(std::get<2>(v)           == "abc");

    v = 21.12;
    assert(std::get<double>(v) == 21.12);
    assert(std::get<3>(v)      == 21.12);
}

int main()
{
    test_copy_assign();
    test_move_assign();
    test_alternative_assign();
}
