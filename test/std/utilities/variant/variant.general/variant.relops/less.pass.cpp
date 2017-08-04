// UNSUPPORTED: c++98, c++03, c++11, c++14

#include <variant>
#include <cassert>

using std::variant;

void test_compile_less()
{
  using test = variant<bool,int,double,std::nullptr_t>;
  constexpr test t1{1};
  constexpr test t2{2};
  constexpr test t3{true};
  static_assert(t1 < t2);
  static_assert(!(t2 < t1));
  static_assert(!(t1 < t3));
}

int main()
{
  test_compile_less();
}
