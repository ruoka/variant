// UNSUPPORTED: c++98, c++03, c++11, c++14

#include <variant>

using std::variant;

struct foo
{
  foo(foo&&) {};
  foo& operator = (foo&&) noexcept {return *this;};
};

int main()
{
  static_assert(std::is_move_constructible_v<foo>);
  static_assert(std::is_move_assignable_v<foo>);
  static_assert(std::is_move_assignable_v<variant<foo>>);
}
