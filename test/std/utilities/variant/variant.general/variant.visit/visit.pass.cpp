// UNSUPPORTED: c++98, c++03, c++11, c++14

#include <variant>
#include <string>
#include <chrono>
#include <iostream>
#include <cassert>

using std::variant;
using std::monostate;
using std::visit;

using test = variant<bool,int,double,std::string>;

struct visitor
{
  bool operator () (bool b1, bool b2, bool b3)
  {
    call = "bool";
    std::cout << call << "\n";
    return true;
  }
  bool operator () (int i1, int i2, int i3)
  {
    call = "int";
    std::cout << call << "\n";
    return true;
  }
  bool operator () (double d1, double d2, double d3)
  {
    call = "double";
    std::cout << call << "\n";
    return true;
  }
  bool operator () (const std::string& str1, const std::string& str2, const std::string& str3)
  {
    call = "string";
    std::cout << call << "\n";
    return true;
  }
  std::string call;
};

void test_functor()
{
  visitor vis;
  test var1, var2, var3;

  var1 = var2 = var3 = true;
  visit(vis, var1, var2, var3);
  assert(vis.call == "bool");

  var1 = var2 = var3 = 13;
  visit(vis, var1, var2, var3);
  assert(vis.call == "int");

  var1 = var2 = var3 = 21.12;
  visit(vis, var1, var2, var3);
  assert(vis.call == "double");

  var1 = var2 = var3 = std::string{};
  visit(vis, var1, var2, var3);
  assert(vis.call == "string");
}

void test_lambda()
{
  std::string call;
  test var1, var2;

  var1 = var2 = true;
  visit([&call](const auto& arg1,const auto& arg2){call += "x";std::cout << arg1 << "\n";}, var1, var2);
  assert(call == "x");

  var1 = var2 = 13;
  visit([&call](const auto& arg1,const auto& arg2){call += "x";std::cout << arg1 << "\n";}, var1, var2);
  assert(call == "xx");

  var1 = var2 = 21.12;
  visit([&call](const auto& arg1,const auto& arg2){call += "x";std::cout << arg1 << "\n";}, var1, var2);
  assert(call == "xxx");

  var1 = var2 = std::string{"Hello!"};
  visit([&call](const auto& arg1,const auto& arg2){call += "x";std::cout << arg1 << "\n";}, var1, var2);
  assert(call == "xxxx");
}

void test_complex()
{
  using type = variant<monostate,
                      std::nullptr_t,
                      double,
                      std::string,
                      bool,
                      std::chrono::system_clock::time_point,
                      long,
                      long long
  >;

  const auto value = type{};

  visit([](const auto& arg){return "pöö";}, value);
}

int main()
{
  test_functor();
  test_lambda();
  test_complex();
}
