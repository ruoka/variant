
// UNSUPPORTED: c++98, c++03, c++11, c++14

#include <variant>
#include <string>
#include <vector>

using std::variant;
using std::variant_size_v;
using std::variant_alternative_t;

int main()
{
  using V = variant<unsigned, double, char, float, std::string, std::vector<int>>;
  static_assert(variant_size_v<V> == 6);
  static_assert(std::is_same_v<variant_alternative_t<5, V>, std::vector<int>>);
}
