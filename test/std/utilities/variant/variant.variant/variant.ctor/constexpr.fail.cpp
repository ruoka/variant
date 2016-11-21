// UNSUPPORTED: c++98, c++03, c++11, c++14

#include <variant>
#include <vector>
#include <cassert>

using std::variant;

int main()
{
  // constexpr variant<bool,int,double,std::vector<int>> v1{};
}
