// UNSUPPORTED: c++98, c++03, c++11, c++14

#include <variant>
#include <vector>
#include <string>
#include <tuple>
#include <cassert>

using std::variant;
using std::get;

struct foo
{
  foo(const foo&) = default;
  foo(foo&&) = default;
};

struct bar
{
  bar() = delete;
  bar(const bar&) = delete;
  bar(bar&&) = delete;
};

static std::size_t s_count = 0;

template<typename T>
struct allocator_tester : std::allocator<T>
{
  allocator_tester() noexcept = default;

  allocator_tester(const allocator_tester&) noexcept = default;

  template <class U> allocator_tester(const allocator_tester<U>& other) :
  std::allocator<T>{other}
  {}

  T* allocate(std::size_t n, const void * hint = nullptr)
  {
    s_count += n;
    return std::allocator<T>::allocate(n);
  }

  void deallocate(T* p, std::size_t n)
  {
    // s_count -= n;
    std::allocator<T>::deallocate(p,n);
  }
};

using my_string = std::basic_string<char, std::char_traits<char>, allocator_tester<char>>;

template<typename T>
using my_vector = std::vector<T, allocator_tester<T>>;

void test_type_traits()
{
  // Default
  static_assert(std::is_constructible_v<variant<int,foo>,  std::allocator_arg_t, std::allocator<char>, foo>);
  static_assert(!std::is_constructible_v<variant<int,bar>, std::allocator_arg_t, std::allocator<char>, bar>);

  // Copy
  static_assert(std::is_constructible_v<variant<int,foo>,  std::allocator_arg_t, std::allocator<char>, variant<int,foo>&>);
  static_assert(!std::is_constructible_v<variant<int,bar>, std::allocator_arg_t, std::allocator<char>, variant<int,bar>&>);

  // Move
  static_assert(std::is_constructible_v<variant<int,foo>,  std::allocator_arg_t, std::allocator<char>, variant<int,foo>&&>);
  static_assert(!std::is_constructible_v<variant<int,bar>, std::allocator_arg_t, const std::allocator<char>&, variant<int,bar>&&>);
}

void test_constructors_1()
{
  using variant = variant<bool,long,double,my_vector<int>,my_string>;
  allocator_tester<variant> a;

  variant v1{std::allocator_arg, a};
  assert(v1.index() == 0);
  assert(get<0>(v1) == false);

  variant v2{std::allocator_arg, a, 1l};
  assert(v2.index() == 1);
  assert(get<1>(v2) == 1);

  variant v3{std::allocator_arg, a, 2.2};
  assert(v3.index() == 2);
  assert(get<2>(v3) == 2.2);

  auto tmp = s_count;

  variant v4{std::allocator_arg, a, std::in_place_type<my_vector<int>>, {1,2,3,4,5,6,7}};
  assert(v4.index() == 3);
  assert(get<3>(v4).size() == 7);
  assert(get<3>(v4)[3] == 4);

  variant v5{std::allocator_arg, a, std::in_place_index<3>, {1,2,3,4,5,6,7,8}};
  assert(v5.index() == 3);
  assert(get<3>(v5).size() == 8);

  variant v6{std::allocator_arg, a, std::in_place_type<my_string>, "foo"};
  assert(v6.index() == 4);
  assert(get<4>(v6) == "foo");

  variant v7{std::allocator_arg, a, std::in_place_index<4>, "bar"};
  assert(v7.index() == 4);
  assert(get<4>(v7) == "bar");

  variant v8{std::allocator_arg, a, std::in_place_type<my_string>, {'a','b','c','d'}};
  assert(v8.index() == 4);
  assert(get<4>(v8) == "abcd");

  variant v9{std::allocator_arg, a, std::in_place_index<4>, std::size_t{1000000}, char{'a'}};
  assert(v9.index() == 4);
  // assert(get<4>(v9) == "aaaaaaaaaa");

  variant v22{std::allocator_arg, a, v2};
  assert(v22.index() == 1);
  assert(get<1>(v22) == 1);

  variant v23{std::allocator_arg, a, std::move(v3)};
  assert(v23.index() == 2);
  assert(get<2>(v23) == 2.2);

  variant v30{std::allocator_arg, a, my_string{"test"}};
  assert(v30.index() == 4);
  assert(get<4>(v30) == "test");

  assert(s_count != 0);
  assert(tmp < s_count);
}

void test_constructors_2()
{
  using variant = variant<long long,my_vector<int>,my_string>;
  allocator_tester<variant> a;

  auto tmp = s_count;

  variant v21{std::allocator_arg, a, short{21}};
  assert(v21.index() == 0);
  assert(get<0>(v21) == 21);

  variant v22{std::allocator_arg, a, my_string(size_t{10000},char{'X'})};
  assert(v22.index() == 2);
  assert(get<2>(v22) == my_string(size_t{10000},char{'X'}));

  variant v23{std::allocator_arg, a, unsigned{23}};
  assert(v23.index() == 0);
  assert(get<0>(v23) == 23u);

  assert(tmp < s_count);
}

void test_constructors_3()
{
  using variant = variant<std::tuple<int,my_vector<int>>,std::tuple<double,my_string>>;
  allocator_tester<variant> a;

  auto tmp = s_count;

  variant v31{std::allocator_arg, a, std::make_tuple(13,my_vector<int>{1,2,3})};
  assert(v31.index() == 0);

  variant v32{std::allocator_arg, a, std::make_tuple(12.34, my_string{"test"})};
  assert(v32.index() == 1);

  assert(tmp < s_count);
}

int main()
{
  test_type_traits();
  test_constructors_1();
  test_constructors_2();
  test_constructors_3();
}
