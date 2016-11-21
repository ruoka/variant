// UNSUPPORTED: c++98, c++03, c++11, c++14

#include <variant>
#include <cassert>

using std::variant;
using std::get;

struct foo{};

int main()
{
  variant<foo,double,char> v1, v2;
  v1 = 1.1;
  v2 = 'A';
  assert(get<1>(v1) == 1.1);
  assert(get<2>(v2) == 'A');
  v1.swap(v2);
  assert(get<1>(v2) == 1.1);
  assert(get<2>(v1) == 'A');
}
