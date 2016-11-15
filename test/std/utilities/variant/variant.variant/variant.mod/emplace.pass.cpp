#include<variant>

static int count = 0;

struct foo
{
    foo(int) {++count;} // count == 1
    ~foo(){++count;} // count == 2
};

int main()
{
    std::variant<bool,foo> v{};
    assert(count == 0);
    v.emplace<foo>(13);
    assert(count == 1);
    v.emplace<bool>(true);
    assert(count == 2);
    v.emplace<foo>(13);
    assert(count == 3);
    v.emplace<bool>(true);
    assert(count == 4);
}
