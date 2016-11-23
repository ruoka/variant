// // UNSUPPORTED: c++98, c++03, c++11, c++14

#include <variant>
#include <functional>
#include <string>

using std::variant;
using std::visit;

struct visitor
{
  int operator () (bool, bool)
  {
    return 0;
  }
  int operator () (const std::string&, const std::string&)
  {
    return 1;
  }
};

int main()
{
  visitor vis;
  variant<bool,std::string> var1, var2;
  bool b1, b2;
  std::string str1, str2;

  b1 = b2 = true;
  std::invoke(vis, b1, b2);

  var1 = var2 = true;
  visit(vis, var1, var2);

  str1 = str2 = "test";
  std::invoke(vis, str1, str2);

  var1 = var2 = "test";
  visit(vis, var1, var2);
}
