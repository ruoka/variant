// UNSUPPORTED: c++98, c++03, c++11, c++14

#include <variant>
#include <cassert>

using std::variant;

static int count = 0;

struct foo
{
  ~foo() = default;
};

struct bar
{
  bar() {++count;} // count == 1
  ~bar(){++count;} // count == 2
};

int main()
{
  static_assert(std::is_trivially_destructible_v<variant<int,foo>> == true);

  static_assert(std::is_trivially_destructible_v<variant<int,bar>> == false);

  assert(count == 0);
  {
    variant<bar> v{std::in_place_type<bar>};
    assert(count == 1);
  }
  assert(count == 2);
}
