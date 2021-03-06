// UNSUPPORTED: c++98, c++03, c++11, c++14

#include <variant>
#include <vector>
#include <cassert>

using std::variant;

int main()
{
  using test = variant<bool,int,double>;
  constexpr test t1{};
  static_assert(t1.index() == 0);

  constexpr test t2{true};
  static_assert(t2.index() == 0);

  constexpr test t3{1};
  static_assert(t3.index() == 1);

  constexpr test t4{2.2};
  static_assert(t4.index() == 2);
}
